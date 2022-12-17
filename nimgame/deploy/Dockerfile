FROM ubuntu:20.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get install -y lib32z1 xinetd
RUN useradd -m ctf

COPY ./flag1 /flag1
COPY ./flag2 /flag2
COPY ./nimgame /pwn/nimgame
COPY ./ctf.xinetd /etc/xinetd.d/ctf

RUN chown root:ctf /pwn/nimgame && chmod 750 /pwn/nimgame
RUN chown root:ctf /flag1 && chmod 644 /flag2
RUN chown root:ctf /flag1 && chmod 644 /flag2
RUN echo 'ctf - nproc 1500' >>/etc/security/limits.conf

CMD exec /bin/bash -c "/etc/init.d/xinetd start; trap : TERM INT; sleep infinity & wait"

EXPOSE 8888 


FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.hit.edu.cn/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get install -y lib32z1 xinetd
RUN useradd -m ctf

RUN apt-get -y install apache2
RUN a2enmod cgid

COPY ./site.conf /etc/apache2/sites-enabled/000-default.conf

COPY ./src /pwn/src
COPY ./doc /pwn/doc

RUN chown root:www-data /pwn/doc && chmod 750 /pwn/doc
RUN chown root:www-data /pwn/src/* && chmod -R 777 /pwn/src/
RUN echo 'ctf - nproc 1500' >>/etc/security/limits.conf

CMD exec /bin/bash -c "/etc/init.d/apache2 start; trap : TERM INT; sleep infinity & wait"

EXPOSE 80 


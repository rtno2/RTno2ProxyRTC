FROM ubuntu:24.04
RUN apt-get update
RUN apt-get install wget git vim curl less emacs bsdmainutils -y

RUN echo "source /usr/share/bash-completion/completions/git" >>~/.bashrc

RUN apt-get install python3 python3-pip python3-tk -y
RUN apt-get update
RUN apt-get install -y --no-install-recommends\
 g++\
 make\
 cmake\
 graphviz\
 omniorb-nameserver\
 libomniorb4-dev\
 omniidl\
 doxygen\
 nkf\
 gnupg2\
 uuid-dev\
 libboost-filesystem-dev\
 python3

RUN apt-get clean


ENV PYTHONUNBUFFERED 1
ENV USER root
RUN mkdir /code
WORKDIR /code
RUN mkdir /code/scripts/
ADD scripts/openrtm2_install_ubuntu.sh /code/scripts/
RUN /code/scripts/openrtm2_install_ubuntu.sh
ADD . /code/


RUN python3 -m pip install fluent-logger rtshell-aist rtctree-aist rtsprofile-aist --break-system-packages
RUN rtshell_post_install -n
ENV PYTHONUNBUFFERED 1

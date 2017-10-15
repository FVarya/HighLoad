FROM fedora

RUN yum install -y gcc-c++
RUN yum install -y boost-devel
RUN yum install -y cmake
RUN yum install -y make

ADD . /app
WORKDIR /app

RUN cmake CMakeLists.txt
RUN make

CMD ./httpd -p 80 -r /var/www/html -n 4

EXPOSE 80

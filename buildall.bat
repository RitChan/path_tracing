@REM Windows
xmake

@REM Debian:11
docker build -f docker/Debian11_Dockerfile -t xmake_debian:11 docker && ^
docker run -it --name xmake --rm -v %CD%:/home/workdir -w="/home/workdir" xmake_debian:11 xmake --root

@REM Ubuntu:20.04
docker build -f docker/Ubuntu2004_Dockerfile -t xmake_ubuntu:20.04 docker && ^
docker run -it --name xmake --rm -v %CD%:/home/workdir -w="/home/workdir" xmake_ubuntu:20.04 xmake --root

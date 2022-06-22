sed -e 's|^mirrorlist=|#mirrorlist=|g' \
    -e 's|^#baseurl=http://mirror.centos.org/$contentdir|baseurl=https://mirrors.ustc.edu.cn/centos|g' \
    -i.bak \
    /etc/yum.repos.d/CentOS-Linux-AppStream.repo \
    /etc/yum.repos.d/CentOS-Linux-BaseOS.repo \
    /etc/yum.repos.d/CentOS-Linux-Extras.repo \
    /etc/yum.repos.d/CentOS-Linux-PowerTools.repo
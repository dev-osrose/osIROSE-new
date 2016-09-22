
sudo apt-get remove mysql-client
sudo apt-get purge mysql-client

echo mysql-apt-config mysql-apt-config/select-server select mysql-5.7 | sudo debconf-set-selections
wget http://dev.mysql.com/get/mysql-apt-config_0.7.3-1_all.deb
sudo dpkg --install mysql-apt-config_0.7.3-1_all.deb
sudo apt-get update -q
sudo apt-get install -q -y -o Dpkg::Options::=--force-confnew mysql-server
sudo mysql_upgrade
#sudo service mysql stop

#echo "CREATE USER 'travis'@'localhost' IDENTIFIED BY '';
#GRANT ALL PRIVILEGES ON *.* TO 'travis'@'localhost' WITH GRANT OPTION;" > ~/mysql-init
#cat ~/mysql-init
#sudo mysqld_safe --init-file=~/mysql-init &
#sleep 5
#rm ~/mysql-init

#sudo /etc/init.d/mysql stop
#sudo /etc/init.d/mysql start

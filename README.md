CMPT 433 Project

![alt text](https://i.imgur.com/ftVQ6PS.png)

Group members:
Ethan Jung
David Tran
Osborn Cheng
Sam Cribb

How to run the project:
 1. Navigate to the webpage directory, and run 'npm install'.
 2. Run the makefile in the main directory by typing 'make'. This will compile an executable by the name of'plant-manager', and a tarball     of the webpage directory. Both of these files will be copied to ~/cmpt433/public/plant-manager/
 3. On the target, navigate to the /plant-manager/ folder on the mounted file system (~/cmpt433/public/ assumed to be mounted)
 4. Run the executable using the command './plant-manager' on the target
 5. In another terminal window, extract 'web.tar.gz' on the target using the command 'tar -zxvf /mnt/remote/plant-manager/web.tar.gz'
 6. Navigate into the extracted webpage folder
 7. Run the server using the command 'node server.js'
 8. Connect to the server on the host's browser by entering into the address bar: :8088

![alt text](https://i.imgur.com/reGYSAQ.png)

Notes:
 - The plant-manager executable can be stopped via the host by running 'netcat -u  12345', and then entering the command 'stop'

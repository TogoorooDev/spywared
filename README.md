# spywared
spywared is a set of daemons for running different spyware activities on a computer.

## NOTICE
The author and contributors to this program take no responsibility for any legal issues a user may run into. This project is a demo and expirement; it is not intended to be used on unauthorized computers.

## Daemons
There are a few daemons that are built into spywared. The default launcher (spywared.exe) starts all of them. These daemons are currently a work in progress, and are not complete yet.

### klogd
klogd is a simple keylogger. It works well on the alphebet, numbers, and the CTRL key; however is not yet working for special characters. It puts keys by default into a keys.txt file in the working directory.

### historyd
historyd takes browser history from Google Chrome. Firstly it checks for an installtion. If one is found, it waits until the Chrome process is not running and takes the URLs visited from ```%LOCALAPPDATA%\Google\Chrome\User Data\Default\History```. It puts these by default into a history.txt file in the working directory.

### passwordd (passwordd branch)
passwordd is similar to historyd, taking saved passwords from Chrome rather than browser history. This is still unfinished, and currently only supports Chrome 80 and below. See the passwordd branch if you would like to test this.

### captured
captured is a screenshot utility. It takes screenshots headlessly. Configuartion is done with the config.h file. Options are the interval, the image type, and the prefix.

### Else
Do you have an idea for a daemon? Please make an issue describing it

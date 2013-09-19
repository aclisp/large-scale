import subprocess
import os

def init():
	os.system('rm -f ~/.ssh/known_hosts')
	os.system('echo "StrictHostKeyChecking no" > ~/.ssh/config')
	# https://help.github.com/articles/generating-ssh-keys
	# ssh-copy-id user@machine

def upload(srcFile, remoteHost, dstDir, userName='root'):
	dest = ''.join([userName,'@',remoteHost,':',dstDir])
	p = subprocess.Popen(["scp", srcFile, dest])
	p.wait()
	return p.returncode

def remote(command, remoteHost, userName='root'):
	dest = ''.join([userName,'@',remoteHost])
	p = subprocess.Popen(["ssh", dest, command])
	p.wait()
	return p.returncode

def bash(command):
	p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	(out, err) = p.communicate()
	return (p.returncode, out, err)

def remotebash(command, remoteHost, userName='root'):
	dest = ''.join([userName,'@',remoteHost])
	p = subprocess.Popen(["ssh", dest, command], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	(out, err) = p.communicate()
	return (p.returncode, out, err)


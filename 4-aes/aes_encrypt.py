import os 
import sys 
import hashlib 
from Crypto.Cipher import AES

def padding(data):
	return data + \
			(AES.block_size - len(data) % AES.block_size) * \
			chr(AES.block_size - len(data) % AES.block_size) 

def encrypt(plaintext, key):

	key = hashlib.sha256(key).digest() 
	iv = 16 * '\x00'
	plaintext = padding(plaintext) 
	
	cipher = AES.new(key, AES.MODE_CBC, iv) 
	ciphertext = cipher.encrypt(bytes(plaintext))
	
	return ciphertext

def display(data):
	s = "{"
	for x in data:
		s += hex(ord(x))
		s += ", "
	s = s[:-2]
	s += "}"
	return s 

def main():

	# prepare plaintext
	filename = sys.argv[1]
	with open(filename, "rb") as f:
		plaintext = f.read() 
	
	# encrypt 
	key = os.urandom(16)       
	ciphertext = encrypt(plaintext, key)
	
	print("key = {}".format(display(key)))
	print("payload = {}".format(display(ciphertext)))

if __name__ == "__main__":
	main()
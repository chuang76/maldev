import sys 

def xor(plaintext, key):

	ciphertext = ""
	
	for i in range(len(plaintext)):
		x = plaintext[i]
		k = key[i % len(key)]
		ciphertext += chr(ord(x) ^ ord(k))
	
	return ciphertext

def display(data):
	# display the ciphertext in hex format 
	s = "{"
	for x in data:
		s += hex(ord(x))
		s += ", "
	s = s[:-2]
	s += "}"
	print(s) 

def main():

	# read plaintext
	filename = sys.argv[1]
	with open(filename, "rb") as f:
		plaintext = f.read() 
	
	# encrypt 
	key = "mykey"
	ciphertext = xor(plaintext, key)
	
	display(ciphertext) 

if __name__ == "__main__":
	main()
	
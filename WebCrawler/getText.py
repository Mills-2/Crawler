from bs4 import BeautifulSoup
import sys
import re
import pprint
import string
import socket
import errno

doc = sys.stdin.read()
soup = BeautifulSoup(doc, "html5lib")

strings = soup.findAll(text=True)

try:
    for s in strings:
        cleanStr=s.strip()
        if(len(cleanStr) > 0):
            print(cleanStr.encode("ascii", "replace"))

    # We close these in the "try" block to avoid
    #   broken pipe errors when the program quits
    sys.stdout.close()
    sys.stderr.close()
    sys.stdin.close()

except socket.error as e:
    # A socket error: that's okay
    x=7;
except IOError as e:
    if e.errno == errno.EPIPE:
        x=7;
    else:
        print("IOError")

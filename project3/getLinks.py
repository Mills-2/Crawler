from bs4 import BeautifulSoup
import sys
import re
import socket
import errno

doc = sys.stdin.read()
soup = BeautifulSoup(doc, "html5lib")

# this version only includes absolute http addresses
links = soup.findAll('a', href=re.compile("^http://|^https://"))

try:
    print(len(links))

    for l in links:

        if l.has_attr('href'):
            print(l['href'])

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

#!/usr/bin/python
# ping_pong1.py

# Programme avec TROUS !!

""" Un programme qui fait ping pong ping pong ... un certain nombre (N) de fois
    N threads ping et N threads pong """

#____________________________________________________
# utilisation des semaphores
#

import sys
from time import sleep
from random import random
from threading import Thread, Semaphore

def ping() :
    global semPing
    global semPong # obligatoire car la variable est m.a.j.
    semPing.acquire()
    print "ping...",
    semPing.release()

def pong() :
    global semPing
    global semPong
    semPong.acquire()
    print "pong"
    semPong.release()

# main thread
if __name__ == '__main__' :
    if sys.argv[1:] : 	# si liste des parametres non vide
        N = int(sys.argv[1])
    else :
        N = 100 	# 100 ping pong par defaut

    semPing = Semaphore(1)
    semPong = Semaphore(1)

    semPing.acquire()
    semPong.acquire()

    # creation d'une liste de N threads ping ; chaque thread fait UN (seul) ping
    t1 = Thread(target=ping)
    # creation d'une liste de N threads pong ; chaque thread fait UN (seul) pong
    t2 = Thread(target=pong)

    t1.start()
    print "pere avant release ping"
    sleep(2)
    semPing.release()
    t1.join()
    
    t2.start()
    print "\npere avant release pong"
    sleep(2)
    semPong.release()
    t2.join()

    print "\nFin de partie !"

# NB: le mutex pourrait aussi etre un parametre des fonctions ping et pong

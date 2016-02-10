import sys, getopt, os

def main(argv):
    inputFile = ''
    #get the opts, if theres an error print usage and close
    try:
        opts, args = getopt.getopt(argv, "i:", ["ifile="])
    except getopt.GetoptError:
        print ('Error: hw2.py -i <ifile>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-i", "--ifile"):
            inputFile = arg
    #open file
    try:
        with open(inputFile, 'r') as fp:
            content = fp.readlines()
    except IOError:
        print ("Could not read file:", inputFile)
        sys.exit(1)
    try:
        fp.close()
    except IOError:
        sys.exit(1)
    #distribute content to appropriate variables
    #order should be
    #n - number of games
    #c - max number of tokens
    #tok - current tokens
    #r - refill amount
    #fun values in separate array
    n = int(content[0])
    c = int(content[1])
    r = int(content[2])
    t = c
    f = []
    total = 0
    for i in range(0, n):
        f.append(int(content[i+3]))

    #process the algorithm
    for i in range(0, n):
        if(i == len(f) - 1): #if the last element in the list
            if(f[i] > 0):
                total += t * f[i]
                t = 0
                break
            else:
                total += f[i]
                t -= 1
                break
        elif(f[i] >= f[i+1]): #if Ai >= Ai+1
            if(f[i] > 0):
                total += t * f[i]
                t = 0
            else: #we have to play atleast one game, negatives are bad for our total fun.
                total += f[i]
                t -= 1
        else: #Ai < Ai+1 #Play one game
            if((t+r-1 > c)): #overflow case
                total += (t+r-c) * f[i]
                t = c - r
            else: #just have one
                total += f[i]
                t -= 1
        t += r #replenish tokens at the end of current game
    #loop ends
    print(total)


if __name__ == "__main__":
    main(sys.argv[1:])

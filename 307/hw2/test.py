#Steven Herring
#109569390
#CSE 307 HW2 - Iterative solution
import sys, getopt, os
maxFun = 0
def getFun(token, game, funCount, fList, refill):
    global maxFun
    if(game == len(fList)):
        return funCount + token * fList[game]
    if(funCount > maxFun):
        maxFun = funCount
    for i in range(1, token-1):
        return getFun(token-i+refill, game+1, funCount, fList, refill)
    return "nothing"

def main(argv):
    global maxFun
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
    inputFile = argv[0]
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
    n = int(content[0][4])
    c = int(content[1][4])
    r = int(content[2][7])
    t = c
    f = []
    total = 0
    for i in range(0, n):
        if(content[i+3][6] == '-'):
            f.append(int(content[i+3][7]) * -1)
        else:
            f.append(int(content[i+3][6]))

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
    sys.stdout.write("total_fun(%d).\n" % (total))
    n = int(content[0][4])
    c = int(content[1][4])
    r = int(content[2][7])
    t = c
    f = []
    total = 0
    for i in range(0, n):
        if(content[i+3][6] == '-'):
            f.append(int(content[i+3][7]) * -1)
        else:
            f.append(int(content[i+3][6]))
    getFun(t, 0, 0, f, r)
    print(maxFun)


if __name__ == "__main__":
    main(sys.argv[1:])

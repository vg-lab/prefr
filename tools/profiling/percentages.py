import os
import sys
import getopt

class File:
    filedesc = None
    path = ""
    id = ""
    elements = None


def main(argv):
    
    
    pattern = "gprof2dot"
    elements = None
    directory = None
        
    try:
        opts, args = getopt.getopt(argv, "p:e:d:", ["pattern", "elements","directory"])
    
    except getopt.GetoptError:        
        sys.exit(2)
            
    for opt, arg in opts:
        if opt in ("-p", "--pattern"):
            pattern = arg
        if opt in ("-e", "--elements"):            
            elements = str(arg).split(";")
        if opt in ("-d", "--directory"):
            directory = str(arg)
            
    if directory == None:
        print "Error: a directory path needs to be provided"
        sys.exit(2)    
        
    if elements == None:
        print "Error: a list of elements (-e <element1;element2;...;elementn>) needs to be provided"
        sys.exit(2)
        
    # LIST FILES
    
    if not directory.endswith("/"):
        directory += "/"
        
    files = os.listdir(directory)
    
    fileList = dict()
    
    for fname in files:        
        if pattern in fname:
            f = File()
            f.path = fname
            f.id = fname.split("-")[-3]
            f.elements = dict()
#             print "%s %s" % (fname, f.id) 
            fileList[f.id] = f
    
    # READ FILES
    errorList = set()
    
    for fk in fileList.keys():
        f = fileList.get(fk)
        f.filedesc = open(directory + f.path)
#         print "%s %s" % (f.id, f.path) 
        for line in f.filedesc:
            for e in elements:
                if e in line and not f.elements.has_key(e):
#                     print e
#                     print line                    
                    percent = line.split("\\n")[-3]
#                     print "%s %s" % (e, percent)
                    f.elements[e] = percent
        
        for e in elements:
            if not e in f.elements.keys():
#                 print "Error: %s not found on file %s" % (e, f.id)                                    
                errorList.add(e)       
    
    if len(errorList) > 0:
        for e in errorList:
            print "\tRemoved '%s' from elements list" % (e)
            elements.remove(e)
    
    print 
    output = "ParticlesNumber"     
    for e in elements:
        output += "," + e.replace(":","")
    print output
            
    for fk in sorted(fileList):
        f = fileList.get(fk)
        output = str(fk)        
        for e in elements:
            if not e in f.elements:
                print "Error: %s not found in %s" % (e, f.id)
                continue
            output += "," + f.elements.get(e)[:-1]            
        print output
                  
    
    
        
     
# Main execution
if __name__ == "__main__":
    main(sys.argv[1:])   
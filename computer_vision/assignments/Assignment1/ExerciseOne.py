'''
AUTHOR: Mark McNulty

PURPOSE: The purpose of this program is to take a list of numbers and return modifications of that list to the user

USAGE:   python3 ExerciseOne.py
         python ExerciseOne.py
'''
# define the repeats function
def repeats(inputList):
    # make a new list with zeros in every element
    repeat = [0]*100
    # loop through the input list
    # increment the value of the element in the repeat list that corresponds to the value in the input list
    for i in inputList:
        repeat[i] += 1
    return repeat
    
# define even numbers in order
def allEvenNumbersInOrder(inputList):
    # sort the list
    inputList.sort()
    # make a new list
    outputList = []
    for i in range(len(inputList)):
        # if the element is even, add it to the output list
        if inputList[i] % 2 == 0:
            outputList.append(inputList[i])
    return outputList

# read in the input file
with open('datafile.txt', 'r') as file:
    data = eval(file.read())


# print the max value in the list
print("max:", str(max(data)))
print()

# print the min value in the list
print("min:", str(min(data)))
print()

# print the index of 38
print("index of 38:", str(data.index(38)))
print()

# print the number of times each number is repeated
# this calls the function repeats
print("number of times each number is repeated: ", end="")
print(repeats(data))
print()

# make a new copy of the list
sortedList = data.copy()

# sort the list
sortedList.sort()

# display the list
print("sorted list:", end="")
print(sortedList)
print()

# make copy of the list
allEven = data.copy()

# print out all even in order
print("all even numbers in order:", end="")

# call the function
print(allEvenNumbersInOrder(allEven))


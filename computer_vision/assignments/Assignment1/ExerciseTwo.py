'''
AUTHOR: Mark McNulty

PURPOSE: This program will take a string as an input and display if the string contains the 'abcd' sequence.
         This is done using a state machine.

USAGE:   python3 ExerciseTwo.py
         python ExerciseTwo.py
'''
# define the states
# define state0
def state0(input):
    # move to stateA if input is a
    if input != 'a':
        return state0
    else:
        return stateA

def stateA(input):
    # move to stateB if input is b
    # stay in stateA if input is a
    # return to state0 if input is not a or b
    if input == "a":
        return stateA
    elif input == "b":
        return stateB
    else:
        return state0

def stateB(input):
    # move to stateC if input is c
    # go back to stateA if input is a
    # return to state0 if input is not a or c
    if input == "a":
        return stateA
    elif input == "c":
        return stateC
    else:
        return state0

def stateC(input):
    # move to stateD if input is d
    # go back to stateA if input is a
    # return to state0 if input is not a or d
    if input == 'a':
        return stateA
    elif input == 'd':
        stateD(input)
        return state0
    else:
        return state0

def stateD(input):
    # print that the string contains the sequence
    print("abcd is contained in the string")
    return None

# create dictionary to describe the state machine
state_dictionary = {
    state0: "No A yet",
    stateA: "state A",
    stateB: "State B",
    stateC: "State C",
    stateD: "State D"
}

# create main loop
if __name__ == "__main__":
    # get the string from the user
    input_string = input("Enter a string: ")

    # make pointer to state 0
    state = state0

    # loop through the string
    i = 0
    while state is not None:
        # call the state function
        new_state = state(input_string[i])

        # move to the next state
        state = new_state

        # break if the end of the string is reached
        if i == len(input_string) - 1:
            break

        # increment the counter
        i += 1

    print("done with state machine")
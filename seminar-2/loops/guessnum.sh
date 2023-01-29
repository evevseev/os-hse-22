#!/bin/bash

# GuessNum.sh
# Script that asks the user to guess a random number

user_guess=

# generate a random number between 1 and 15
correct_answer=$(( ( RANDOM % 15 ) + 1 ))

# loop until the user guesses the number
while [ "$user_guess" != "$correct_answer" ]; do 
  read -r -p "Enter your guess: " user_guess
  
  # check if the guess is too high or too low
  # and print the corresponding message
  if [ "$user_guess" -gt "$correct_answer" ]; then 
    echo "Too high!"
  elif [ "$user_guess" -lt "$correct_answer" ]; then
    echo "Too low!"
  fi
done

echo "Congrats! You won!"
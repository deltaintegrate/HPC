#!/bin/bash
	for l in 10 20 30 40 50 60 70 80 90 100 200 300 400 500 600 700 800 900 1000 1500 2000 2500 3000 3500 4000
		do
		for k in 1 2 3 4 5 6 7 8 9 10
			do
			./cache $l >>resultado.txt
			sleep 1
			done
		done

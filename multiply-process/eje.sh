#!/bin/bash

			for l in  10 20 30 40 50 60 70 80 90 100 200 300 400 500 600 700 800 900 1000 
				do
				./process $l >> resultado.txt
				echo acabado
				done
				

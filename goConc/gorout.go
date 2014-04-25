package main

import (
	"fmt"
	"time"
)

var sem = make(chan int)
var sem2 = make(chan int)

func printI(i int) {
	fmt.Println(i)
	sem <- 1
}

func main(){
	
	go func() {
		for i:=0;i<10;i++ {
			<-sem
		}
		sem2 <- 1
	}()

	for i:=0;i<10;i++ {
		i:=i
		go func() {
			fmt.Print(i);
			time.Sleep(200*time.Millisecond)
			printI(i)
		}()
			time.Sleep(50*time.Millisecond)
	}
		
	<-sem2
}

package main

import (
	"fmt"
	"net"
	"os"
	"io"
	"bufio"
)

func main() {
	if len(os.Args) !=2 {
		fmt.Fprintf(os.Stderr,"Usage wrong\n")
		os.Exit(0)
	}	
	
	addr:= os.Args[1]
/*	tcpAddr,err:=net.ResolveTCPAddr("tcp4",addr)
	checkError(err)
*/	
	conn,err:= net.Dial("tcp",addr)
	checkError(err)

//	_,err= conn.Write([]byte("Hello\r\n"))
//	checkError(err)
	sync :=make(chan int)

	go func(c net.Conn) {
		for {
			bio:= bufio.NewReader(os.Stdin)
			line,_,err:=bio.ReadLine()
			checkError(err)
			io.WriteString(c,string(line))
		}
		sync<-1
	}(conn)
	<-sync
	os.Exit(0)
}

func checkError(err error) {
	if(err!=nil) {
		fmt.Fprintf(os.Stderr,"Error: %s",err.Error())
	}
}

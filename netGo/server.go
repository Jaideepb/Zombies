package main

import (
	"fmt"
	"net"
	"os"
	"io/ioutil"
)

func main(){
	l,err := net.Listen("tcp","127.0.0.1:4444")
	checkError(err)

	defer l.Close()

	for {
		con,err:= l.Accept()
		checkError(err)
		go func(c net.Conn) {
		//	var b []byte
		//	n,err:=c.Read(b)


			res,err:=ioutil.ReadAll(c)
			checkError(err)


//			fmt.Fprintf(os.Stdout,"Read: %d bytes, %s",n,string(b))
			fmt.Println(string(res))
			c.Close()
		}(con)	
	}
}

func checkError(err error) {
	if(err!=nil) {
		fmt.Fprintf(os.Stderr,"Error: %s",err.Error())
	}
}

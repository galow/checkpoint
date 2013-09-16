myplotData <- function() {
	rigiddata <- read.table("result1.txt")
	plot(rigiddata,xlab="deviation",ylab="correlation",main="D-C",ylim=c(0,1),xlim=c(0.08,0.25),col="#0000000f",cex = 1,pch=16)
	#points(rigiddata,pch=20,col="red")
	points(0.129099,0.745478,pch = 20,cex = 2.2,col = "blue")
	arrows(0.1,0.75,0.129099,0.745478,length = 0.1,angle = 25,col="brown",lwd = 2)
}

myplotData2 <- function() {
	rigiddata <- read.table("result2.txt")
	plot(rigiddata,xlab="basia of size",ylab="correlation",main="B-C",ylim=c(0.1,1),xlim=c(200,550),col="#0000000f",cex = 1,pch=16)
	#points(rigiddata,pch=20,col="red")
	points(416,0.745478,pch = 20,cex = 2.2,col = "blue")
	arrows(480,0.9,416,0.745478,length = 0.1,angle = 25,col="brown",lwd = 2)
}
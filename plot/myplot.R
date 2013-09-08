myplotData <- function() {
	rigiddata <- read.table("result.txt")
	plot(rigiddata,xlab="B",ylab="W",main="B-W",ylim=c(0,0.4),xlim=c(0,2),cex = 1,pch=20,col="green")
	#points(rigiddata,pch=20,col="red")
	#points(0.53033,0.155812,pch = 20,cex = 2.2,col = "brown")
}

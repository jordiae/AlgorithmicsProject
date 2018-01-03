all: programV1 programV2 Dinic FordFulkerson EdmondsKarp v1Dinic v1FordFulkerson v1EdmondsKarp v2Dinic v2FordFulkerson v2EdmondsKarp
programV1:
	g++ -DVERSION=1 -c program.cc -o programV1.o
programV2:
	g++ -DVERSION=2 -c program.cc -o programV2.o
Dinic:
	g++ -c Dinic.cpp -o Dinic.o
FordFulkerson:
	g++ -c FordFulkerson.cpp -o FordFulkerson.o
EdmondsKarp:
	g++ -c EdmondsKarp.cpp -o EdmondsKarp.o
v1Dinic:
	g++ -o v1Dinic.x programV1.o Dinic.o
v1FordFulkerson:
	g++ -o v1FordFulkerson.x programV1.o FordFulkerson.o
v1EdmondsKarp:
	g++ -o v1EdmondsKarp.x programV1.o EdmondsKarp.o
v2Dinic:
	g++ -o v2Dinic.x programV2.o Dinic.o
v2FordFulkerson:
	g++ -o v2FordFulkerson.x programV2.o FordFulkerson.o
v2EdmondsKarp:
	g++ -o v2EdmondsKarp.x programV2.o EdmondsKarp.o
clean:
	rm *.o
	rm *.x


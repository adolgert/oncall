# You have to call this with
# TOKEN=apitoken make
oncall: main.cpp
	g++ -g -DTOKEN=$(TOKEN) -I/usr/local/boost-current/include main.cpp -o oncall -lcurl

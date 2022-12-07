dependency:
	cd build && cmake .. --graphviz=graph.dot && -Tpng graph.dot -o graphImage.png
prepare:
	rm -rf build
	mkdir build
	cd build && cd ..
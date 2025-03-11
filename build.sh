# #!/bin/bash
# cd ..

# if [ -d "build" ]; then
#     echo "Build directory already exists."
#     rm -r build
# else
#     echo "Created build directory."
# fi

# mkdir build
# cd build
# cmake ../work
# make -j2

# ��workĿ¼�������
#!/bin/bash

if [ -d "build" ]; then
    echo "Build directory already exists."
    rm -r build
else
    echo "Created build directory."
fi

mkdir build
cd build
cmake ..
make -j2
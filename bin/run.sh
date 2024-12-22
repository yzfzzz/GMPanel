if [ -d "logs" ]; then
    echo "logs directory already exists."
    rm -r logs
else
    echo "Created logs directory."
fi

mkdir logs
python3 ./utils/write.py & ./monitor -i address.conf
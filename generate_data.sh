
# Function to handle cleanup and exit
cleanup_exit() { 
    echo "Data generation stopped by user"
    exit 0
}

# Trap SIGINT (Ctrl+C) and SIGTERM (termination signal) to call cleanup_exit function
trap 'cleanup_exit' SIGINT SIGTERM

while true; do
    # Find USB drive path dynamically
    usb_drive=$(find /C/ -maxdepth 2 -type d -name "*" -print -quit)
    #usb_drive=$(lsblk -o MOUNTPOINT,NAME -r | grep "^/media/usb" | awk '{print $1}')
    #usb_drive=$(lsblk -o MOUNTPOINT,NAME,TYPE -r | awk '$3=="C" && $1 ~ "/media/usb" {print $1; exit}')
    if [ -z "$usb_drive" ]; then
        echo "USB drive not found"
        sleep 5
        continue
    fi
    
    # Create directory if not exist
    directory="$usb_drive/Users/VA185105"
    if [ ! -d "$directory" ]; then
        mkdir "$directory"
        if [ $? -ne 0 ]; then
            echo "Error: Failed to create directory $directory"
            sleep 5
            continue
        fi
    fi
    
    # Define file path
    file_path="$directory/data.txt"

    # Check if the file exists, if not, create it
    if [ ! -f "$file_path" ]; then
        touch "$file_path"
        if [ $? -ne 0 ]; then
            echo "Error: Failed to create file $file_path"
            sleep 5
            continue
        fi
    fi

    # Attempt to generate data and write to file
    data=$(shuf -i 0-100 -n 1)  # Generate random number between 0 and 100
    if [[ $? -eq 0 ]]; then  # Check if shuf command succeeded
        echo "$data" >> "$file_path"
    else
        echo "Error: Failed to generate data"
    fi

    sleep 5  # adjust interval as needed
done
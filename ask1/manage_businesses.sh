#!/bin/bash

###############################################
# 1093513 Christodoulopoulos Eftathios Panagiotis
# 1093514 Christodoulou Nikolaos
# _______ Basilopoulos Basilios
###############################################

# Default file name
DEFAULT_FILE="Businesses.csv"

# [1] Function to select a business file
select_business_file() {
    read -p "Enter the path of the business file or press enter to use default: " file_path
    if [[ -z "$file_path" ]]; then
        file_path=$DEFAULT_FILE
    fi
    if [[ ! -f "$file_path" ]]; then
        echo "File not found. Creating a new file: $file_path"
        touch "$file_path"
    fi
    echo "Selected file: $file_path"
}

# [2] Function to view business details
view_business_details() {
    read -p "Enter the business code: " business_code
    grep "^$business_code," "$file_path"
}

# [3] Function to change a business element
change_business_element() {
    read -p "Enter the business code: " business_code
    IFS=',' read -ra fields <<< $(grep "^$business_code," "$file_path")
    if [[ ${#fields[@]} -eq 0 ]]; then
        echo "Business not found."
        return
    fi

    echo "Select the field to change:"
    echo "1) BusinessName 2) AddressLine2 3) AddressLine3 4) PostCode 5) Longitude 6) Latitude"
    read -p "Choice: " choice

    if ((choice < 1 || choice > 6)); then
        echo "Invalid choice."
        return
    fi

    read -p "Enter the new value: " new_value
    old_value=${fields[$choice]}
    fields[$choice]=$new_value
    new_record=$(IFS=,; echo "${fields[*]}")

    # Use a temporary file for sed operation
    tmp_file=$(mktemp)
    sed "/^$business_code,/c$new_record" "$file_path" > "$tmp_file" && mv "$tmp_file" "$file_path"
    echo "Changed from $old_value to $new_value"
}

# [4] Function to view the file
view_file() {
    less "$file_path"
}

# [5] Function to save the file
save_file() {
    read -p "Enter the path to save the file or press enter for default: " save_path
    if [[ -z "$save_path" ]]; then
        save_path="clients.csv"
    fi
    cp "$file_path" "$save_path"
    echo "File saved to $save_path"
}

# Main menu
while true; do
    echo "----------------------------------------"
    echo "Menu:"
    echo "1) Select business file"
    echo "2) View business details"
    echo "3) Change business element"
    echo "4) View file"
    echo "5) Save file"
    echo "0) Exit"
    read -p "Enter your choice: " choice

    case $choice in
        1) select_business_file ;;
        2) view_business_details ;;
        3) change_business_element ;;
        4) view_file ;;
        5) save_file ;;
        0) break ;;
        *) echo "Invalid choice. Please try again." ;;
    esac
done

echo "Program terminated."

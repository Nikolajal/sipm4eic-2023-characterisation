#!/bin/bash

# Default Variables
output_file="presentation.tex"
presentation_title="Figures Presentation"
author_name="eic"
date="\\today"
figures_directory="figures"
compile_pdf=false
compfiles_directory="compfiles_directory"
pdf_output_dir="."                                                                                   # Default folder for final PDF output
number_list=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32) # Default list of numbers
data_folder="\"/home/eic/DATA/2023-characterisation/actual/\""
database_file="\"/home/eic/alcor/alcor-utils/measure/2023-characterisation/database/database.txt\""
OUT_DIR="\".\""
target_macro="history.C"
OVERVOLTAGE=4

# Function to display usage
usage() {
    echo "Usage: $0 [-o output_file] [-t presentation_title] [-a author_name] [-d date] [-f figures_directory] [-c] [-b compfiles_directory] [-p pdf_output_dir] [-n 'number1 number2 ...']"
    echo "  -o, --outfile            Output.tex file"
    echo "                           Def: $output_file"
    echo "  -t, --title              Presentation title"
    echo "                           Def: $presentation_title"
    echo "  -a, --author             Author name"
    echo "                           Def: $author_name"
    echo "  -d, --date               Date"
    echo "                           Def: $date"
    echo "  -f, --figures            Directory containing the figures"
    echo "                           Def: $figures_directory"
    echo "  -c, --compile            Compile .tex to PDF using pdflatex"
    echo "                           Def: $compile_pdf"
    echo "  -b, --compfiles          Directory to store LaTeX compilation files"
    echo "                           Def: $compfiles_directory"
    echo "  -p, --pdfdir             Directory to save the final PDF"
    echo "                           Def: $pdf_output_dir"
    echo "  -n, --numbers            Space-separated list of numbers (default: 1 2 ... 32)"
    exit 1
}

# Parse command-line arguments
while [[ "$1" != "" ]]; do
    case $1 in
    -o | --outfile)
        shift
        output_file=$1
        ;;
    -t | --title)
        shift
        presentation_title=$1
        ;;
    -a | --author)
        shift
        author_name=$1
        ;;
    -d | --date)
        shift
        date=$1
        ;;
    -f | --figures)
        shift
        figures_directory=$1
        ;;
    -c | --compile)
        compile_pdf=true
        ;;
    -b | --compfiles)
        shift
        compfiles_directory=$1
        ;;
    -p | --pdfdir)
        shift
        pdf_output_dir=$1
        ;;
    -n | --numbers)
        shift
        IFS=' ' read -r -a number_list <<<"$1" # Override default with provided list
        ;;
    -h | --help)
        usage
        ;;
    *)
        usage
        ;;
    esac
    shift
done

# Debug: Print the list of numbers (or process them as needed)
if [ ${#number_list[@]} -gt 0 ]; then

    # Loop through each number in the number_list array
    for number in "${number_list[@]}"; do
        mkdir -p ./figures/
        echo "Processing number: $number" # Debug print to see which number is being processed
        root -l -q -b $target_macro\($data_folder,$database_file,\"$number\",$OVERVOLTAGE,true\)
        root -l -q -b $target_macro\($data_folder,$database_file,\"$number\",$OVERVOLTAGE,false\)

        # Check if ROOT command was successful
        if [ $? -ne 0 ]; then
            echo "Error running ROOT macro for number $number."
            exit 1
        fi
    done
else
    echo "[ERROR] At least one board needs to be provided"
    exit 1 # Exit with an error code
fi

# Check if ROOT command was successful
if [ $? -ne 0 ]; then
    echo "Error running ROOT macro."
    exit 1
fi

./make_figures_presentation.sh -c

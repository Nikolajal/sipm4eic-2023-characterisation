#!/bin/bash

# Default Variables
output_file="presentation.tex"
presentation_title="Figures Presentation"
author_name="eic"
date="\\today"
figures_directory="figures"
compile_pdf=false
compfiles_directory="./compfiles_directory"
pdf_output_dir="." # Default folder for final PDF output

# Function to display usage
usage() {
    echo "Usage: $0 [-o output_file] [-t presentation_title] [-a author_name] [-d date] [-f figures_directory] [-c] [-b compfiles_directory] [-p pdf_output_dir]"
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
        -h | --help)
            usage
            ;;
        *)
            usage
            ;;
    esac
    shift
done

# Start the LaTeX document
cat <<EOF > "$output_file"
\documentclass{beamer}
\usepackage{graphicx}

\title{$presentation_title}
\author{$author_name}
\date{$date}

\begin{document}

\frame{\titlepage}

EOF

# Initialize counter for figures per frame
figures_per_frame=0

# Loop through all image files in the specified directory and add them to the .tex file
for figure in "$figures_directory"/*.{png,jpg,jpeg,pdf}; do
    if [ -f "$figure" ]; then
        # Check if it's the start of a new frame (every two figures)
        if [ $figures_per_frame -eq 0 ]; then
            echo "\begin{frame}" >> "$output_file"
            echo "  \frametitle{$figure}" >> "$output_file"
            echo "  \begin{center}" >> "$output_file"
        fi

        # Add the figure to the current frame
        echo "    \includegraphics[width=\textwidth]{$figure}" >> "$output_file"
        figures_per_frame=$((figures_per_frame + 1))

        # Check if two figures have been added (end of frame)
        if [ $figures_per_frame -eq 2 ]; then
            echo "  \end{center}" >> "$output_file"
            echo "\end{frame}" >> "$output_file"
            figures_per_frame=0  # Reset counter for the next frame
        fi
    fi
done

# Close the last frame if there's an odd number of figures
if [ $figures_per_frame -eq 1 ]; then
    echo "  \end{center}" >> "$output_file"
    echo "\end{frame}" >> "$output_file"
fi

# End the LaTeX document
cat <<EOF >> "$output_file"
\end{document}
EOF

echo "LaTeX presentation file '$output_file' has been created."

# Compile the LaTeX file to PDF if the --compile flag is provided
if [ "$compile_pdf" = true ]; then
    echo "Compiling .tex to PDF..."

    # Ensure the compilation directory exists
    mkdir -p "$compfiles_directory"

    # Run pdflatex with options to store compilation files in a specific directory
    pdflatex -output-directory="$compfiles_directory" -interaction=nonstopmode "$output_file"

    # Check if PDF compilation was successful
    if [ $? -eq 0 ]; then
        # Move the compiled PDF to the specified output directory
        compiled_pdf="$compfiles_directory/$(basename "${output_file%.tex}.pdf")"
        if [ -f "$compiled_pdf" ]; then
            mkdir -p "$pdf_output_dir"
            mv "$compiled_pdf" "$pdf_output_dir/"
            echo "PDF compiled successfully: $pdf_output_dir/$(basename "$compiled_pdf")"
        else
            echo "Failed to find the compiled PDF file."
            exit 1
        fi
    else
        echo "PDF compilation failed."
        exit 1
    fi
fi

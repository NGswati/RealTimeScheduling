# import csv


# def convert_txt_to_csv(txt_file, csv_file):
#     with open(txt_file, 'r') as file:
#         lines = file.readlines()

#     with open(csv_file, 'w', newline='') as file:
#         writer = csv.writer(file)
#         for line in lines:
#             row = line.strip().split()  
#             writer.writerow(row)


# txt_file = 'tempresult.txt' 
# csv_file = 'tempresult.csv'  

# convert_txt_to_csv(txt_file, csv_file)

import csv

input_file = "tempresult.txt"
output_file = "tempresult.csv"

data = []
current_set = ""
headers = ["Pc_id", "peak_temp", "Avg_temp"]


with open(input_file, "r") as file:
    lines = file.readlines()

    for line in lines:
        line = line.strip()
        if line.startswith("Set"):
            current_set = line
        elif line and not line.startswith("Pc_id"):
            values = line.split()
            data.append([current_set] + values)


with open(output_file, "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["Set", "Pc_id", "peak_temp", "Avg_temp"])
    writer.writerows(data)

print("Conversion completed successfully.")

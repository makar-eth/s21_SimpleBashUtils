import os
from itertools import permutations
import time

options_list = 'vetbns'
files_list = ["../data-samples/111.txt", "../data-samples/222.txt", "../data-samples/test2.txt", \
            "../data-samples/error.txt", "../data-samples/file", "../data-samples/test.txt", \
            "../data-samples/bytes.txt"]
success_counter = 0
fail_counter = 0

def option_gen(options_list):
    for i in range(1, len(options_list)):
        for option in permutations(options_list, i):
            yield option

def files_gen(files_list):
    for j in range(1, len(files_list)):
        for files in permutations(files_list, j):
            yield files

log = open("log.txt", 'w')
for option in option_gen(options_list):
    # files_g = files_gen(files_list)
    for file in files_list:
        os.system(f"cat -{''.join(option)} {file} > cat.txt")
        os.system(f"./s21_cat -{''.join(option)} {file} > s21_cat.txt")
        os.system("diff -s s21_cat.txt cat.txt > res.txt")

        f = open("res.txt", 'r')
        res = f.read()
        print(res)

        if res == "Files s21_cat.txt and cat.txt are identical\n":
            success_counter += 1
        else:
            log.write(f"cat -{''.join(option)} {file}\n")
            fail_counter += 1
        os.system("rm s21_cat.txt cat.txt res.txt")

print(f"SUCCESS: {success_counter}")
print(f"FAIL: {fail_counter}")
import math

digit_permutation_lookup = [
    lambda n: n,
    lambda n: n*(n+1)/2,
    lambda n: n*(n*(n+3)+2)/6,
    lambda n: n*(n*(n*(n+6)+11)+6)/24,
    lambda n: n*(n*(n*(n*(n+10)+35)+50)+24)/120,
    lambda n: n*(n*(n*(n*(n*(n+15)+85)+225)+274)+120)/720
]

criterion_fail_lookup = [83, 21, 6, 1]

def dynamic_range(n, up):
    print("-------------")
    range = 0
    digits = str(n)
    if up:
        for i, d in enumerate(reversed(digits)):
            if i == len(digits)-1: break;
            diff = 9 - int(d) if int(d) > 0 else 9
            range_diff = digit_permutation_lookup[i](diff)
            range += range_diff
    else:
        for i, d in enumerate(digits):
            if i == 0: continue
            diff = 9 - int(d) + 1 if int(d) > 0 else 9
            range_diff = digit_permutation_lookup[5-i](diff)
            range += digit_permutation_lookup[5-i](9)-range_diff
    print(range)
    return range


def in_range(lower, upper):
    lower_order = int(math.log(lower, 10))
    lower_MSD = int(lower/pow(10, lower_order))
    upper_order = int(math.log(upper, 10))
    upper_MSD = int(upper/pow(10, upper_order))

    flat_range = digit_permutation_lookup[5](9-lower_MSD) - \
                 digit_permutation_lookup[5](9-upper_MSD+1)
    if lower_MSD < 4: flat_range -= criterion_fail_lookup[lower_MSD]
    print(flat_range)
    lower_range = dynamic_range(lower, False)
    upper_range = dynamic_range(upper, True)
    return lower_range + flat_range + upper_range


print(in_range(264360, 746325))


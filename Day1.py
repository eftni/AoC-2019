import math

def sumfuel():
    with open("input.txt") as file:
        nums = file.readlines()
    sum = 0
    for n in nums: sum += int(int(n) / 3) - 2
    return sum

def recurse(num):
    if int(num/3)-2 < 1:
        return 0
    res = int(num/3)-2
    return res + recurse(res)


def recfuel():
    with open("input.txt") as file:
        nums = file.readlines()
    sum = 0
    for n in nums:
        sum += recurse(int(n))
        '''first = int(int(n)/3)-2
        print(first, end=' ')
        print(recurse(int(n)), end=' ')
        ##res = int(1.5*first-first/pow(3, int(math.log(first,3))+1))
        res = int(1.5*first-round(2*math.log(first,3))-2.5-4)
        print(res, end=' ')
        print(res - recurse(int(n)))'''
    return sum

def constfuel():
    with open("input.txt") as file:
        nums = file.readlines()
    sum = 0
    for n in nums: sum += int(1.5*(int(int(n)/3)-2)-2*math.log((int(int(n)/3)-2),3)-9)
    return sum
'''
TL;DR az alján

Na ez érdemel egy kis magyarázatot:
Azt a sort, hogy szumma ((((x/3)-2)/3-2)/3-2)/3-2....
fel lehet írni úgy is, hogy szumma x/(3^n) - 2/(3^n-1) - 2/(3^n-2) - ... - 2/3 - 2
Ahol az összes 2/(3^n-k) tag elhanyagolható (asszem), hiszen 2/(3^n) n>0 kisebb, mint 1, amit elhanyagolunk 
Ezért az egész úgy néz ki, hogy x/(3^n) - 2
Na most, ha szumma(n) = A, akkor szumma(x*n) = x*A. It is known.
Mivel szumma(1/3^n) = 3/2, ezért szumma(x/3^n) = 3x/2 = 1.5*x, szóval a teljes üzemanyagszükséglet 1.5*üzemanyag-2
De ez nekünk még nem elég, mert ez túl sok (azért is, mert mi nem a végtelenig szummálunk, csak log3(x)-ig, de ez legfeljebb 0.999999... eltérést jelenthet, hiszen x/(3^(log3(x)+1))) definíció szerint kisebb, mint 1)

A Wolfram alpha szerint szumma(x/(3^n)-2) ahol n=0->log3(x) = 1.5*(3x*5)-2*log3(x)
https://www.wolframalpha.com/input/?i=sum&assumption=%7B%22F%22%2C+%22Sum%22%2C+%22sumlowerlimit%22%7D+-%3E%220%22&assumption=%7B%22C%22%2C+%22sum%22%7D+-%3E+%7B%22Calculator%22%7D&assumption=%7B%22F%22%2C+%22Sum%22%2C+%22sumfunction%22%7D+-%3E%22x%2F%283%5Ek%29-2%22&assumption=%7B%22F%22%2C+%22Sum%22%2C+%22sumvariable%22%7D+-%3E%22k%22&assumption=%7B%22F%22%2C+%22Sum%22%2C+%22sumupperlimit2%22%7D+-%3E%22log3%28x%29%22
Nem ellenőriztem le, mert lusta voltam.
Minden esetre ez a lefelé kerekítések miatt minden elemre +p üzemanyagot ad ki, ahol 0 < p < log3(x), bár heurisztikusan és ködös matektudásra alapozva ez inkább log3(x)/2 < p < log3(x)
(Ha minden igaz, akkor a lefelé kerekítésekben elveszett egészek száma soha nem lehet kisebb, mint log3(x)/2, mert something something hárommal való osztás tulajdonságai.
Nem voltam hajlandó mélyen belemenni, mert van egy olyan érzésem, hogy ennek valahogy a Collatz-sejtéshez van köze, és ha a világ vezető matematikusai ~73 éve nem tudták megoldani, akkor nekem esélyem sincs.
Bár az is lehet, hogy semmi köze ehhez, és nálam csak beütött az autizmus.)

Minden esetre azt nem tudom, hogy konstans időben meg lehet-e határozni, hogy hány olyan osztás van a sorban, ahol a lefelé kerekítés miatt elveszik egy egész.
És itt jön be a Mátyus-állandó: Ha heurisztikus alapon minden erdményből kivonok log3(x)-et, akkor az eredmény
4925580
Ami a várt
4925608
hoz képest +28, azaz 0.00056% eltérés.
Ez lefedi a "wishing really hard" részt.
A törtpék meg elmehetnek a fenébe, hogy lefelé kerekítenek.

Aki ezt megolja nekem, annak jövök egy sörrel (absztinencia esetén tábla csokival).
Lehet, hogy sok itt a hibás feltételezés, de már így is túl sok energiát fektettem egy 10 perces adventi progi feladatba.

TL;DR
Autism happened, ended up with possible Collatz-conjecture derivative, added "heuristic" constant to get almost-good result.
#Engineering
'''



print(sumfuel())
print(recfuel())
print(constfuel())
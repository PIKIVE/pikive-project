from math import *
group = list()
def k_mean(user,k):
    global group
    group = [ (a,sim[a]) for a in range(0,user_max)] #유사한 K명의 그룹
    group.sort(key=lambda tup:tup[1],reverse=True)
    group = group[:k]
    group = list(map(lambda tup : tup[0], group))
    """
    for mem in group :
        print(clients[mem])
    """
    rcm_list = []
    rcm_items = list(books-mat[clients[user]])# user에게 추천하고자 하는 item
    for item in rcm_items :
         choice = [ 1 for mem in group if item in mat[clients[mem]]]
         score = sum(choice)
         rcm_list.append((item,score))
    rcm_list.sort(key=lambda tup:tup[1],reverse=True)
    rcm_list = rcm_list[:10]
    i=1
    for book in rcm_list :
        print(str(i) + " : "+book[0])
        i+=1
    

    """
 


    for item in rcm_items :
        a = [mat[mem].get(item) for mem in group if item in mat[mem].keys()]
        print((sum(a)+1)/(len(a)+2)) # smoothing
    """
def cosine_distance(user) :
    for i in range(0,user_max) :
        if i == user :
            continue
        calc = 0
        asq = 0
        for item in mat[clients[user]] :
            asq += 1
            if item not in mat[clients[i]]:
                continue
            calc += 1
        asq = sqrt(asq)
        bsq = 0
        for item in mat[clients[i]]  :
            bsq += 1
        bsq = sqrt(bsq)

        calc /= (asq*bsq)
        sim[i] = round(calc,2)
clients = set()
books = set()
mat = {}

    
with open("buy.txt","r") as f:
    for i in f.readlines():
        line = i.split("\t")
        if len(line)==2 :
            name, item = line
        else :
            name, item, pub = line
        clients.add(name)
        books.add(item)
        if name not in mat.keys() :
            mat[name] = set()
            mat[name].add(item)
        else :
            mat[name].add(item)
f.close()
user_max = len(clients)
sim = [0 for i in range(0,user_max)]
clients = list(clients)

name = "이경미"
user = clients.index(name) #name이란 이름을 가지는 타깃의 배열에서의 인덱스값
cosine_distance(user)
k_mean(user,30)

books = list(books)
books.sort()
print("group : ")
print(group)

with open("sample.txt","w") as f:
    for name in group:
        for book in mat[clients[name]] :
            f.write(' '+str(books.index(book)))
        f.write('\n')
f.close()

choose = [ 357,518,522,546,548,550,555,567,4872]
for book in choose :
    print(books[book])



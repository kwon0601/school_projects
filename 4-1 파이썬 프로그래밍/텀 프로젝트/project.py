import numpy as np
import math
import random
import sys

dictionary={}#각 파일로 만들어진 클래스를 저장할 dictionary자료형
use_default=1#로또에서 로또수와 반복수를 default값으로 할지
default_ticket_num=1000#기본 로또수(설정에서 변경가능)
default_iteration=2000#로또에서 기본 반복수(설정에서 변경가능)
name_default=1#이름을 파일이름으로 자동 설정할지

#어레이 길이에 상관없이 파일에 출력되게 변경
np.set_printoptions(threshold=np.inf,linewidth=np.inf)
#----------------vclass나 관련 함수들v------------------
class data():
    mode=0 #모드 (1:statistics, 2:lottery)
    data=[] #원본 데이터
    section_division=[] #데이터 구분 구간
    data_result=[] # 구간에 따른 데이터 개수
    name=""#이름
    form=0#표준:1 log:2
    def __init__(self,data,mode,name,skip):
        if (skip==0):
            self.data=data
            self.mode=mode
            self.name=name
            self.section_division,self.data_result,self.form=statistics(data)
        
    def print_result(self):   
        if(sum(self.data_result)==0):
            print("데이터가 없습니다.")
        else:
            if(self.mode==1):
                print(self.name,"의 통계")
            if(self.form==1):
                print("구분 구간 :표준")
            else:
                print("구분 구간: Log")
            print("{0:^15s} {1:>12s} {2:>20s}".format("구간","값","퍼센티지"))
            percentage=[]
            for i in range(0,len(self.data_result)):
                percentage.append((self.data_result[i]/sum(self.data_result))*100)
            for i in range(0,len(self.data_result)):
                if(i==len(self.data_result)-1):
                    print("[%7s,%7s] :%7d"%(number_format(self.section_division[i]),number_format(self.section_division[i+1]),(self.data_result[i])),end="")
                else:
                    print("[%7s,%7s) :%7d"%(number_format(self.section_division[i]),number_format(self.section_division[i+1]),(self.data_result[i])),end="")
                print(f"    {percentage[i]:5.2f}%",end="")
                if(50<max(percentage)):
                    print("■"*(round(percentage[i]/4)))#■가 1줄에 50개 이상 출력해야 하면 반의반만 출력되게 함
                elif(25<max(percentage)):
                    print("■"*(round(percentage[i]/2)))#■가 1줄에 25개 이상 출력해야 하면 반만 출력되게 함
                else:
                    print("■"*round(percentage[i]))
            print("평균:",number_format(np.nanmean(self.data)))
            print("표준편차:",number_format(np.nanstd(self.data)),"\n")

    def get_data(self):
        return self.data

class lottery_data(data):
    probability_col=0 #확률 열
    ticket_num=1000 #사는 복권 수
    iteration=2000 #시뮬레이션 수
    
    def __init__(self,data,mode,name,skip):
        if (skip==0):
            #어느 열이 확률 열인지 확인
            if(0.999<=(data.sum(axis=0))[0]and(data.sum(axis=0)[0]<=1.001)):
                self.probability_col=0
            else:
                self.probability_col=1
            
            self.data,self.ticket_num,self.iteration=lottery(data,self.probability_col) #시뮬레이션을 해서 결과 값을 받음
            super().__init__(self.data,mode,name,skip)
    def print_result(self):
        print(self.name,"의 통계")
        print(f"복권 수:{self.ticket_num}장 시뮬레이션 수: {self.iteration}번")
        super().print_result()

def number_format(number): #숫자를 단위에 따라 만,억,조를 붙이는 함수
    if((10**12)<number):
        number=str(round(number/(10**12),1))+"조"
    elif((10**8)<number):
        number=str(round(number/(10**8),1))+"억"
    elif((10**4)<number):
        number=str(round(number/(10**4),1))+"만"
    else:
        number=str(round(number,1))
    return number

def statistics(data,types=""):#데이터를 통계내는 함수
    data=data.flatten()#1~2차원 numpy데이터를 1차원으로 전환

    #log값을 쓸지 않쓸지 결정
    if(min(data)<=0):
        data_type="normal"
    elif((max(data))/(min(data))<100):
        data_type="normal"
    else:
        data_type="log"
        log_value=((max(data))/(min(data)))**(1/10) #log 밑 수


    #주어진 값이 있다면, 그것을 사용
    if(min(data)>0):
        if(types=="normal"):
            data_type="normal"
        elif(types=="log"):
            data_type="log"
            log_value=((max(data))/(min(data)))**(1/10) #log 밑 수
   #데이터 값 구분 지점 생성
    section_division=[0]*11     
    if(data_type=="normal"):
        for i in range(0,11):
            form=1
            section_division[i]=min(data)+(((max(data))-(min(data)))/10)*(i)
    elif(data_type=="log"):
        for i in range(0,11):
            form=2
            section_division[i]=min(data)*log_value**(i)

    #구간에 따라 개수 세기
    data_result=[0]*10
    for data_unit in data:
        if(section_division[0]<=data_unit) and (data_unit<section_division[1]):
            data_result[0]+=1
        elif(section_division[1]<=data_unit) and (data_unit<section_division[2]):
            data_result[1]+=1
        elif(section_division[2]<=data_unit) and (data_unit<section_division[3]):
            data_result[2]+=1
        elif(section_division[3]<=data_unit) and (data_unit<section_division[4]):
            data_result[3]+=1
        elif(section_division[4]<=data_unit) and (data_unit<section_division[5]):
            data_result[4]+=1
        elif(section_division[5]<=data_unit) and (data_unit<section_division[6]):
            data_result[5]+=1
        elif(section_division[6]<=data_unit) and (data_unit<section_division[7]):
            data_result[6]+=1
        elif(section_division[7]<=data_unit) and (data_unit<section_division[8]):
            data_result[7]+=1
        elif(section_division[8]<=data_unit) and (data_unit<section_division[9]):
            data_result[8]+=1
        elif(section_division[9]<=data_unit) and (data_unit<=section_division[10]):
            data_result[9]+=1
    return section_division,data_result,form
    
def lottery(data,probability_col):#시뮬레이션을 해서 결과 결과값을 주는 함수
    global use_default,default_ticket_num,default_iteration

    #오른쪽 열이 확률열이면 열을 바꿔 왼쪽 열이 확률을 나타내게 수정
    if (probability_col==1):
        for i in range(0,len(data)):
            data[i][0],data[i][1]=data[i][1],data[i][0]

    #확률의 값이 정확히 1이 되도록 재설정
    probality_sum=data.sum(axis=0)[0]
    for i in range(0,len(data)):
        data[i][0]/=probality_sum

    #확률을 누적으로 바꿈
    for i in range(1,len(data)):
        data[i][0]+=data[i-1][0]

    result=[]

    #복권 수와 시뮬레이션 수를 설정
    if(use_default==1):
        ticket_num,iteration=default_ticket_num,default_iteration
    else:
        ticket_num,iteration= get_simulation_detail()
        
    for i in range(0,iteration): 
        ocurrance=[0]*len(data) #시뮬레이션에서 로또가 등수 별로 몇번 됬는지
        for j in range(0,ticket_num):
            random_num=random.random()
            for k in range(0,len(data)): # 몇 등이 됬는지 판별
                if(random_num<data[k][0]):
                    ocurrance[k]+=1
                    break
        winning_money=0
        for j in range(0,len(data)):
            winning_money+=ocurrance[j]*data[j][1]
        result.append(winning_money)
    return np.array(result),ticket_num,iteration

def get_simulation_detail():#복권 수와 시뮬레이션 수를 입력 받는 함수
    while(True):
        try:
            ticket_num, iteration=input("티켓 수, 시뮬레이션 횟수 입력: (예:2000,1000)").split(",")
        except:#인자가 2개가 아닐때
            print("2개의 숫자를 입력해주세요")
            continue
        try:
            ticket_num=eval(ticket_num)
            iteration=eval(iteration)
        except:#숫자가 아닐때
            print("숫자를 입력해주세요")
        else:
            if ((not(isinstance(ticket_num,int)))or(not(isinstance(iteration,int)))):#정수가 아닐때
                print("정수를 입력해주세요")
                continue
            if((1<=ticket_num) and (1<=iteration)):
                break
            else: #양수가 아닐때
                print("양수를 입력해주세요")
                continue
    return ticket_num, iteration

#----------------^class나 관련 함수들^------------------
#----------------v1.새 파일 생성 관련 함수들v------------------  
def new_file_read():#새로운 파일을 읽어 dictionary에 추가하는 함수
    global dictionary
    while(True):
        file,file_name=read_file() #파일은 파일안 데이터
        mode=get_number("모드 선택(1:statistics,2:lottery)>>>",1,2)
        print()
        if(np.isnan(file).all()):#파일안 내용이 없을때
            print("파일 안에 데이터가 없습니다.\n다른 파일을 입력해주세요\n")
            continue
        
        is_correct=correct_data(file,mode)
        if(is_correct==1):#statistics mode인데 lottery인거 같은 데이터
            want_to_change=input("lottery파일인거 같습니다..\n모드를 바꾸겠습니까? 원한다면 \"yes\"를 치세요 if you want")
            if((want_to_change.lower()=="yes")or(want_to_change.lower()=="y")or(want_to_change=="ㅛ")or(want_to_change=="ㅛㄷㄴ")):
                mode=2
            break
        elif(is_correct==2): #lottery mode인데 조건에 맞지 않는 데이터
            print("파일 형식이 lottery에 맞지 않습니다. 다시 시도 해주세요")
            continue
        elif(is_correct==3): #lottery mode인데 nan 데이터가 있는 경우
            print("Lottery 모드는 숫자가 아닌 데이터가 있으면 안됩니다.")
            continue
        else: #올바른 데이터
            break
            
    if (mode==1):
        new_data = data(file,mode,file_name,0) 
    else:
        new_data = lottery_data(file,mode,file_name,0)  
    dictionary[file_name]=new_data#파일 목록에 추가
    return file_name

def read_file():#파일명을 입력받아 그 안에 data를 리턴하는 함수
    global dictionary,name_default
    while(True):
        file_name=input("파일 이름 입력>>>")
        if(not(file_name.endswith(".csv"))):#.csv를 쓰지 않아도 자동 인식
           file_name+=".csv"

        # CSV 파일 경로 설정(만약 파일 위치가 코드 위치랑 다르면 ''부분 수정)
        filepath = ''+file_name      
        try: # CSV 파일 읽어들이기
            file = np.genfromtxt(filepath, delimiter=',', missing_values=[''])
        except FileNotFoundError:#파일이 없을때
            print("파일이 없습니다.")
            continue

        if(name_default==1):
            save_file_name=file_name[:-4] #파일 이름에서.csv를 뺀것이 기본 값 설정
        else:
            save_file_name=input("리스트에 보여질 이름을 입력하세요>>>")

        #이름이 잘못 설정 됬을때 다시 입력 받음  
        while(True):
            if((save_file_name not in dictionary) and ("," not in save_file_name)):
                break
            if("," in save_file_name): #,포함
                save_file_name=input("이름은 \",\"을 포함할 수없습니다.\n리스트에 보여질 이름을 입력하세요>>>")
                continue
            elif(save_file_name in dictionary):#이미 동일한 이름이 있음
                print(f"{save_file_name} 이 이미 있습니다.")
                save_file_name=input("리스트에 보여질 이름을 입력하세요>>>")
                continue
        break

    return file,save_file_name      

def get_number(sentance,x,y): #선택한 구간내의 정수를 받는 함수
    while(True):
        number=input(sentance)
        try:
            number=eval(number)
        except:#숫자가 아닐때
            print("숫자를 입력해주세요")
        else:
            if (not(isinstance(number,int))):#정수가 아닐때
                print("정수를 입력해주세요")
                continue
            if((x<=number) and (number<=y)):
                break
            else: #주어진 범위의 수가 아닐때
                print(f"[{x},{y}]범위의 정수를 입력해주세요")
                continue
    return number

def correct_data(data,mode): #모드에 알맞은 데이터 인지 확인하는 함수

    #lottery에 해당하는지 판별, 해당하면 1이 됨
    lottery=0
    dimention=data.ndim
    if (dimention==1): #1차원일때
        pass
    elif(0.999<(data.sum(axis=0))[0]and(data.sum(axis=0)[0]<1.001)):
        lottery=1
    elif(0.999<(data.sum(axis=0))[1]and(data.sum(axis=0)[1]<1.001)):
        lottery=1

    nan_elements = data[np.isnan(data)]
    if(0<len(nan_elements))and(mode==2): return 3 #lottery mode인데 nan값이 있는경우
    
    #데이터 판단 결과 리턴   
    if (((mode==1)and(lottery==0))or((mode==2)and(lottery==1))):#알맞은 데이터
        return 0
    elif((mode==1)and(lottery==1)): #statistics mode인데 lottery인거 같은 데이터
        return 1
    elif((mode==2)and(lottery==0)): #lottery mode인데 조건에 맞지 않는 데이터
        return 2        
#----------------^1.새 파일 생성 관련 함수들^------------------
#----------------v2.파일 보기 관련 함수들v------------------ 
def show_statistics(): #원하는 파일 통계를 보는 함수
    global dictionary
    print()
    if(len(dictionary)==0):#저장된 파일이 없을때
        print("저장된 파일이 없습니다. 타이틀로 돌아갑니다.\n")
        return
    while(True):
        #현재 dictionary 목록을 보여주기
        print("\n파일 리스트")
        for file in dictionary:
            print(file)
        print()  

        #보고 싶은 파일 선택 후출력
        input_sentence=input("어떤 파일을 보고 싶습니까? (나가고 싶으면 exit를 입력하세요)\n비교를 원한다면 2개를 쓰세요 (예:file1,file2)")
        #파일 비교
        try:
            view_file1,view_file2=input_sentence.split(",")
            if(view_file1 not in dictionary):
                print("다음 파일이 없습니다:",view_file1)
                continue
            elif(view_file2 not in dictionary):
                print("다음 파일이 없습니다:",view_file2)
                continue
            else:
                data1=dictionary.get(view_file1)
                data2=dictionary.get(view_file2)
                if(data1.mode!=data2.mode):#두 파일의 모드가 다를 시 그만 둘지 확인
                    inputs=input("두 파일의 모드가 다릅니다. 그만두고 싶으면 yes를 입력하세요")
                    if((inputs.lower()=="yes")or(inputs.lower()=="y")or(inputs=="ㅛ")or(inputs=="ㅛㄷㄴ")):
                        continue
                if(data1.name==data2.name):#두 파일이 같을 시 그만 둘지 확인
                    inputs=input("두 파일이 같습니다. 그만두고 싶으면 yes를 입력하세요")
                    if((inputs.lower()=="yes")or(inputs.lower()=="y")or(inputs=="ㅛ")or(inputs=="ㅛㄷㄴ")):
                        continue
                coop_show(data1,data2)
                continue
        #하나의 파일 또는 나가기       
        except:
            if((input_sentence.lower()=="exit")or(input_sentence.lower()=="e")or(input_sentence=="ㄷ")or(input_sentence=="ㄷ턋")):
                print()
                break
            if(input_sentence in dictionary):
                data=dictionary.get(input_sentence)
                data.print_result()
                while(True):
                    try:
                        input_sentence2=input("일정 구간에 있는 자료의 수를 보고싶으면 12,24와 같이 입력하세요.\n(필요없으면 아무거나 입력)").split(",")
                        range_start=eval(input_sentence2[0])
                        range_end=eval(input_sentence2[1])
                        count=0
                        for element in data.data:
                            if((range_start<=element) and (element<=range_end)): count+=1
                        print(f"[{range_start},{range_end}]의 자료수:{count},퍼센트:{100*count/len(data.data)}%\n")
                    except:
                        break     
            else:
                print("해당하는 이름을 가진 파일이 없습니다..")


def coop_show(data1,data2): #2개의 파일을 함께 통계내는 함수 (입력은 data class2개)

    #한쪽이 데이터가 없는 경우
    if((len(data1.get_data())==0)):
        print(f"{data1.name} 가 데이터가 없어서 출력할 수 없습니다.")
        return 
    elif((len(data2.get_data())==0)):
        print(f"{data2.name}가 데이터가 없어서 출력할 수 없습니다.")
        return 
    
    # class안 자료를 1차원으로 바꾼뒤 붙힘
    data1_pressed=np.ravel(data1.get_data())
    data2_pressed=np.ravel(data2.get_data())
    data_concentrated = np.concatenate((data1_pressed, data2_pressed))

    #데이터 구간 생성
    if(data1.form==data2.form):
        if(data1.form==1):
            section_division,trash,form=statistics(data_concentrated,"normal") 
        else:
            section_division,trash,form=statistics(data_concentrated,"log")           
    else:
        section_division,trash,form=statistics(data_concentrated)

    #만들어진 기준에 따라 데이터 개수 세기
    data_result_1=[0]*10
    for data_unit in data1_pressed:
        if(section_division[0]<=data_unit) and (data_unit<section_division[1]):
            data_result_1[0]+=1
        elif(section_division[1]<=data_unit) and (data_unit<section_division[2]):
            data_result_1[1]+=1
        elif(section_division[2]<=data_unit) and (data_unit<section_division[3]):
            data_result_1[2]+=1
        elif(section_division[3]<=data_unit) and (data_unit<section_division[4]):
            data_result_1[3]+=1
        elif(section_division[4]<=data_unit) and (data_unit<section_division[5]):
            data_result_1[4]+=1
        elif(section_division[5]<=data_unit) and (data_unit<section_division[6]):
            data_result_1[5]+=1
        elif(section_division[6]<=data_unit) and (data_unit<section_division[7]):
            data_result_1[6]+=1
        elif(section_division[7]<=data_unit) and (data_unit<section_division[8]):
            data_result_1[7]+=1
        elif(section_division[8]<=data_unit) and (data_unit<section_division[9]):
            data_result_1[8]+=1
        elif(section_division[9]<=data_unit) and (data_unit<=section_division[10]):
            data_result_1[9]+=1  
    data_result_2=[0]*10
    for data_unit in data2_pressed:
        if(section_division[0]<=data_unit) and (data_unit<section_division[1]):
            data_result_2[0]+=1
        elif(section_division[1]<=data_unit) and (data_unit<section_division[2]):
            data_result_2[1]+=1
        elif(section_division[2]<=data_unit) and (data_unit<section_division[3]):
            data_result_2[2]+=1
        elif(section_division[3]<=data_unit) and (data_unit<section_division[4]):
            data_result_2[3]+=1
        elif(section_division[4]<=data_unit) and (data_unit<section_division[5]):
            data_result_2[4]+=1
        elif(section_division[5]<=data_unit) and (data_unit<section_division[6]):
            data_result_2[5]+=1
        elif(section_division[6]<=data_unit) and (data_unit<section_division[7]):
            data_result_2[6]+=1
        elif(section_division[7]<=data_unit) and (data_unit<section_division[8]):
            data_result_2[7]+=1
        elif(section_division[8]<=data_unit) and (data_unit<section_division[9]):
            data_result_2[8]+=1
        elif(section_division[9]<=data_unit) and (data_unit<=section_division[10]):
            data_result_2[9]+=1
            
    
    print(data1.name,"과",data2.name,"의 통계")
    if(form==1):
        print("구분 구간 :표준")
    else:
        print("구분 구간: Log")
    print(data1.name,":■",data2.name,":★")
    print("{0:^15s} {1:>12s} {2:>20s}".format("범위","값","퍼센테이지"))
    percentage_1=[]
    percentage_2=[]
    blank=""
    for i in range(0,len(data_result_1)):
        percentage_1.append((data_result_1[i]/sum(data_result_1))*100)
    for i in range(0,len(data_result_2)):
        percentage_2.append((data_result_2[i]/sum(data_result_2))*100)
        
    for i in range(0,len(data_result_1)):
        if(i==len(data_result_1)-1):
            print("[%7s,%7s] :%7d"%(number_format(section_division[i]),number_format(section_division[i+1]),(data_result_1[i])),end="")
        else:
            print("[%7s,%7s) :%7d"%(number_format(section_division[i]),number_format(section_division[i+1]),(data_result_1[i])),end="")
        print(f"    {percentage_1[i]:5.2f}%",end="")
        if(50<max(percentage_1)):
            print("■"*(round(percentage_1[i]/4)))#■가 1줄에 50개 이상 출력해야 하면 반의반만 출력되게 함
            print(f"{blank:19s}{(data_result_2[i]):7d}    {percentage_2[i]:5.2f}%",end="")
            print("★"*(round(percentage_2[i]/2)))
        elif(25<max(percentage_1)):
            print("■"*(round(percentage_1[i]/2)))#■가 1줄에 25개 이상 출력해야 하면 반만 출력되게 함
            print(f"{blank:19s}{(data_result_2[i]):7d}    {percentage_2[i]:5.2f}%",end="")
            print("★"*(round(percentage_2[i]/2)))
        else:
            print("■"*(round(percentage_1[i])))
            print(f"{blank:19s}{(data_result_2[i]):7d}    {percentage_2[i]:5.2f}%",end="")
            print("★"*(round(percentage_2[i])))
        print("")
            
    print(data1.name,"의 평균:",number_format(np.nanmean(data1_pressed)))
    print(data1.name,"의 표준편차:",number_format(np.nanstd(data1_pressed)))
    print(data2.name,"의 평균:",number_format(np.nanmean(data2_pressed)))
    print(data2.name,"의 표준편차:",number_format(np.nanstd(data2_pressed)))

    while(True):
        try:
            input_sentence2=input("일정 구간에 있는 자료의 수를 보고싶으면 12,24와 같이 입력하세요.\n(필요없으면 아무거나 입력)").split(",")
            range_start=eval(input_sentence2[0])
            range_end=eval(input_sentence2[1])
            count_1=0
            count_2=0
            for element in data1.data:
                if(range_start <= element <= range_end):
                    count_1+=1
            for element in data2.data:
                if(range_start <= element <= range_end):
                    count_2+=1
            print(f"[{range_start},{range_end}]의 자료수\n")
            print(f"{data1.name}:{count_1},퍼센트:{100*count_1/len(data1.data)}%\n")
            print(f"{data2.name}:{count_2},퍼센트:{100*count_2/len(data2.data)}%\n")
        except:
            break
    
#----------------^2.파일 보기 관련 함수들^------------------     
#----------------v3.파일 관리 관련 함수들v------------------

def file_management():#파일 관리하는 함수
    global dictionary
    while(True):
        print()
    
        #현재 dictionary 목록을 보여주기
        print("\n파일 리스트")
        for (file,data) in dictionary.items():
            if(data.form==1):
                print(f"{file:20s}일반")
        for file,data in dictionary.items():
            if(data.form==2):
                print(f"{file:20s}log")
        print()
        
        manage_select_num=get_number("(파일 관리)실행할 것 선택\n1:파일 삭제\n2:파일 이름변경\n3:파일 구분 구간 형식 변경\n4:txt로 저장\n5:txt 불러오기\n6:나가기\n",1,6)
        if(manage_select_num==1):#1:파일 삭제
            delete_file=input("\n삭제 할 파일을 입력해주세요\n,:전부 삭제\n삭제를 원하지 않는다면 리스트에 없는 아무 문자나 입력")
            if(delete_file==","):#파일 전체삭제
                for delete_file in dictionary:
                    print(f"\'{delete_file}\'",end="")
                dictionary.clear()
                print("삭제됨")
                    
            elif(delete_file in dictionary):
                dictionary.pop(delete_file,"")
                print(f"{delete_file} 삭제됨")
        elif(manage_select_num==2):#2:파일 이름변경
            rename_file=input("\n이름을 바꿀 파일을 입력해주세요\n원하지 않는다면 리스트에 없는 아무 문자나 입력")
            if(rename_file in dictionary):
                print(f"새로운 이름: {rename_file}>>> ",end="")
                new_name=input("")
                if (new_name not in dictionary):
                    dictionary[rename_file].name=new_name
                    dictionary[new_name] = dictionary[rename_file]
                    del dictionary[rename_file]
                    print(f"이름 변경 완료:{rename_file}>>>{new_name}")
                else:#해당 이름이 이미 있을때
                    print(f"이미 존재하는 이름:{new_name}")
                    continue
        elif(manage_select_num==3):#3:파일 구분 구간 형식 변경
            reform_file=input("\n파일 구분 구간 형식을 바꿀 파일을 입력해주세요\n원하지 않는다면 리스트에 없는 아무 문자나 입력")
            if(reform_file in dictionary):
                data=dictionary.get(reform_file)
                if(data.form==1):#현재 일반일때
                    data.section_division,data.data_result,data.form=statistics(data.get_data(),"log")#현재 일반일때
                    if(data.form==2):
                        print(reform_file,"의 파일 구분 구간 형식이 log로 변경되었습니다.\n")
                    else:
                        print("0이하의 데이터가 있는경우 log로 변경할 수없습니다.")
                else:
                    data.section_division,data.data_result,data.form=statistics(data.get_data(),"normal")#현재 log일때
                    print(reform_file,"의 파일 구분 구간 형식이 일반으로 변경되었습니다.\n")
                    
            else:#해당이름의 파일이 없을때
                print("해당하는 이름을 가진 파일이 없습니다.")
            
        elif(manage_select_num==4):#4:txt로 저장
            file_save()
        elif(manage_select_num==5):#5:txt 불러오기
            file_load()
        elif(manage_select_num==6):#6:나가기
            break

def file_save():#현재 파일들을 txt파일로 저장
    global dictionary,use_default,default_ticket_num,default_iteration,name_default
    name_list=[]#기존 파일에 더하는 경우에서 이미 있는 파일(instance)이름과 새로운 파일(instance)이름이 겹칠경우 이름을 바꾸고 저장을 하기 위해 기존파일에 있는 파일(instance)이름들을 저장 
    delete_range=[]#위의 경우에 기존 파일에서 삭제해야할 줄수를 기록하는 list
    inputs=0

    #만들 파일명과 기존 파일이 있을경우 처리받음
    output_name=input("생성될 txt파일 이름>>>")
    if(not(output_name.endswith(".txt"))):#.txt를 쓰지 않아도 자동 인식
        output_name+=".txt"
    try:
        file=open(output_name,"r")
        print(output_name,"이 이미 존재합니다. 어떻게 하시겠습니까?")
        print("1.덮어쓰기\n2.기존 파일에 덧붙히기(중복은 덮어쓰기) \n3.기존 파일에 덧붙히기(중복은 건너뜀)")
        inputs=get_number("",1,3)
        if((inputs==2)or(inputs==3)):#기존 파일에 더해서 만듬
            #존재하는 파일(instance)이름들을 name_list저장
            for line in file:
                if (line.startswith("#")):
                    line_splited=line.split()
                    name_list.append(line_splited[0][1:])
            file.close()
            file=open(output_name,"a")
        else:
            a=0/0#except로 가기위한 오류
    except:
        file=open(output_name,"w")
        #현재 세팅들 저장
        file.write(str(use_default)+"\n")
        file.write(str(default_ticket_num)+"\n")
        file.write(str(default_iteration)+"\n")
        file.write(str(name_default)+"\n")

    if(inputs==2):
        #몇번째 줄을 지워야 할지 찾음
        line_number=0#읽고 있는 줄의수
        deleting_now=0
        file.close()
        file=open(output_name,"r")
        for line in file:
            (line_number)+=1
            if ((line.startswith("#") and (deleting_now==1))):
                delete_range.append(line_number)         
            if ((line.startswith("#"))and(line.split()[0][1:] in name_list)):
                delete_range.append(line_number)
                deleting_now=1
        if(len(delete_range)//2==1):
            delete_range.append(-1)
        #해당하는 줄을 지움
        lines = file.readlines()
        for i in range((int)(len(delete_range)/2)-1,-1,-1):
            if(delete_range[2*i+1]==-1):
                del lines[delete_range[2*i]:]
            else:
                del lines[delete_range[2*i]:delete_range[2*i+1]]
        file=open(output_name,"w")
        file.writelines(lines)
        file=open(output_name,"a")    
            
    #dictionary안의 파일들(instance) 파일로 쓰기
    for name,data in dictionary.items():
        if ((name in name_list) and(inputs==3)): #이전에 동일한 이름이 있고,replcaing X
            continue
        file.write("#"+name+"\nmode:"+str(data.mode)+"\n")
        file.write("data:"+str(data.data)+"\n")
        file.write("section_division:"+str(data.section_division)+"\n")
        file.write("data_result:"+str(data.data_result)+"\n")
        file.write("form:"+str(data.form)+"\n")
        if(data.mode==2):#lottery
            file.write("probality_col:"+str(data.probability_col)+"\n")
            file.write("ticket_num:"+str(data.ticket_num)+"\n")
            file.write("iteration:"+str(data.iteration)+"\n")
        file.write("\n")
    file.close()
    print("파일 저장 완료")


def file_load():#txt파일을 읽어서 settings와 파일(instance)정보를 읽음
    global dictionary,use_default,default_ticket_num,default_iteration,name_default
    #파일명 받기
    file_name=input("txt 파일이름>>>")
    if(not(file_name.endswith(".txt"))):#.txt를 쓰지 않아도 자동 인식
        file_name+=".txt"
    try:
        file=open(file_name,"r")
    except:#파일이 없는 경우
        print("해당하는 이름을 가진 파일이 없습니다.")
        return

    #파일을 읽고 내용으로 새로운 instance를 만들어 dictionary에 넣음
    passed=0
    line_number=0#현재 읽고 있는 줄수
    try:
        for line in file:
            line_number+=1
            if(line_number==1):
                use_default=int(line.strip())
            elif(line_number==2):
                default_ticket_num=int(line.strip())
            elif(line_number==3):
                default_iteration=int(line.strip())
            elif(line_number==4):
                name_default=int(line)
            elif (line.startswith("#")):
                name=line.strip()[1:]
            elif (line.startswith('mode:')):
                mode=int(line.strip().split(":")[1])
            elif (line.startswith('data:')):
                values = line.split(':')[1].replace('[','').replace(']','').split()
                datas = np.array(values, dtype=float)
            elif (line.startswith("section_division:")):
                section_division = [float(value) for value in line.strip().split(":")[1].replace("[","").replace("]","").split(",")]
            elif (line.startswith("data_result:")):
                data_result = [int(value) for value in line.strip().split(":")[1].replace("[","").replace("]","").split(",")]
            elif (line.startswith("form:")):
                form=int(line.strip().split(":")[1])
                if(mode==1):
                    if name in dictionary:#만약 같은 이름이 이미 있다면 스킵
                        passed+=1
                    else:
                        #데이터가 너무 길면 끊어서 읽어버리므로(10000개가 있어도 10개정도만읽음) 100개씩 끊어서 데이터추가
                        new_data=data(np.array([1, 1]),1,name,1)
                        for i in range(0, len(datas), 100):
                            new_data.data = np.append(new_data.data, datas[i:i+100])
                        new_data.name=name
                        new_data.mode=mode
                        new_data.section_division=section_division
                        new_data.data_result=data_result
                        new_data.form=form
                        dictionary[name]=new_data
                            
            elif (line.startswith("probality_col:")):
                probality_col=int(line.strip().split(":")[1])
            elif (line.startswith("ticket_num:")):
                ticket_num=int(line.strip().split(":")[1])
            elif (line.startswith("iteration:")):
                iteration=int(line.strip().split(":")[1])
                if name in dictionary:#만약 같은 이름이 이미 있다면 스킵
                    passed+=1
                else:
                    new_data=lottery_data(np.array([1, 1]),2,name,1)
                    new_data.data=datas
                    new_data.name=name
                    new_data.mode=mode
                    new_data.section_division=section_division
                    new_data.data_result=data_result
                    new_data.form=form
                    new_data.probality_col=probality_col
                    new_data.ticket_num=ticket_num
                    new_data.iteration=iteration
                    dictionary[name]=new_data
            else:#data가 여러 줄에 걸쳐도 읽기
                values = line.replace('[','').replace(']','').split()
                datas = np.append(datas, np.array(values, dtype=float))
        if(passed!=0):
            print("이미 동일한 이름이 존재해서 통계 파일 추가",passed,"개 건너뜀(추가되지않음)")
        file.close()
    except Exception as e:#파일 안 내용이 이상할때
        print("txt 내용이 잘못되었습니다.\n",e)
        return 
       
#----------------^3.파일 관리 관련 함수들^------------------ 
#----------------v4.설정 관련 함수들v------------------       

def settings():#여러 세팅을 할 수있는 함수
    global use_default,default_ticket_num,default_iteration,name_default
    print()
    while(True):
        print("(설정)수정할 것 선택?")
        print("1:lottery에서 기본값 사용 여부:","사용중" if use_default==1 else "사용 X")
        print("2:lottery 기본값 수치:",default_ticket_num,default_iteration)
        print("3:파일 자동 이름 설정 여부:","자동" if name_default==1 else "수동")
        print("4:나가기")
        setting_select_num=get_number("",1,4)
        if(setting_select_num==1):#1:lottery에서 기본값 사용 여부
            if(use_default==1):#현재: default사용
                use_default=0
            else:#현재:default 사용 X
                use_default=1
        elif(setting_select_num==2):#2:lottery 기본값 수치
            default_ticket_num,default_iteration= get_simulation_detail()
        elif(setting_select_num==3):#3:파일 자동 이름 설정 여부
            if(name_default==1):#현재:자동 이름
                name_default=0
            else:#현재:수동 이름
                name_default=1
        elif(setting_select_num==4):#4:나가기
            print()
            break
        print()
#----------------^4.설정 관련 함수들^------------------      
#----------------v main v------------------           

def main():
    global dictionary
    while(True):
        main_select_num=get_number("실행할 것 선택\n1:새 파일 생성\n2:파일 보기\n3:파일 관리\n4:설정\n5:종료\n",1,5)
        if(main_select_num==1):# new file     
            file_name=new_file_read()
            (dictionary.get(file_name)).print_result()
        elif(main_select_num==2):# view file statistics
            show_statistics()
        elif(main_select_num==3):# file management
            file_management()
        elif(main_select_num==4):# settings
            settings()
        elif(main_select_num==5):# exit
            break
    print("프로그램이 종료되었습니다.")

main()

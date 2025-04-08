#include<iostream>
#include<string>
#include<windows.h>
#include<stdlib.h>
#include<vector>
#include<iomanip>
#include<fstream>
using namespace std;
//用户名和密码
const string userName="20220001";
const string passWord="123456";
class Goods
{
private:
    string numbering;
    string name;  
    string unit;      
    double price;       
    int stock;
    string date;
    vector<Goods> goodsList;
public:
    Goods(){};
    Goods(string Numbering,string Name,string Unit,double Price,int Stock,string Date)
    {
        numbering=Numbering;
        name=Name;  
        unit=Unit;      
        price=Price;       
        stock=Stock;
        date=Date;
    };
    void addGoods();
    void DeleteGoods();
    void sell();
    void shipping();
    void proInformation();
    void actionLog();
    void inventory();
    void signOut();
    bool checkExist(string numbering)
    {
        for(int i=0;i<goodsList.size();i++)
        {
            if(goodsList[i].numbering==numbering){
                return true;
            }
        }
        return false;
    };
    void storage();
    void outbound();
    void newInf();//商品新增并保存在一个文件中
    void changeInf();
    void deleteInf();
    void addOperate(string Numbering,string Name,string DAte);
};
Goods goods;
int temp=0;
int N;
void beforeLogin();//登录前
//关于商品的操作函数
//新增商品
void Goods::addGoods() 
{
    string fileName="D:/c++/Me/GoodsInf.txt";
    ifstream input(fileName);
    ofstream output(fileName, ios::app);
    if (!input.is_open() || !output.is_open())
    {
        cout << "无法打开文件！" << endl;
        return;
    }
    bool isNewProduct = true;
    cout << "请输入新商品信息:" << endl;
    cout<<"编号：";
    cin>>numbering;
    /*for (const auto& goods : goodsList) 
    {
        if (goods.numbering == numbering) 
        {
            cout << "商品已经存在，请重新选择操作"<< endl;
            return;
        }
    }*/
    while (getline(input, goods.name))
    {
        string line;
        getline(input, line);

        if (goods.numbering == "")
        {
            break;
        }

        if (goods.numbering == line)
        {
            cout << "商品已经存在，请重新选择操作" << endl;
            isNewProduct = false;
            break;
        }
    }

    if(isNewProduct)
    {
        cout<<"名称: ";
        cin>>name;
        cout<<"单位：";
        cin>>unit;
        cout<<"单价: ";
        cin >> price;
        cout<<"库存: ";
        cin>>stock;
        cout<<"日期(xxxx-xx-xx):";
        cin>>date;
        cin.ignore();// 忽略输入缓冲区中的换行符
        goodsList.push_back({numbering,name,unit,price,stock,date});
        goods.newInf();
        goods.addOperate(numbering,name,date);
        output.close();
        cout<<"已成功新增商品"<<endl;
        
    }
    input.close();
    output.close();
}
//删除商品
void Goods::DeleteGoods()
{
    cout << "请输入要删除的商品信息:" << endl;
    cout<<"编号：";
    cin>>numbering;  
    bool found=false;
    for(auto it = goodsList.begin();it!=goodsList.end();it++)
    {
        if(it->numbering==numbering)
        {
            goodsList.erase(it);//如果是要删除的商品则返回true
            found=true;
            break;
        }
    }
    if(found)
    {
        goods.deleteInf();
        cout<<"已成功删除商品"<<endl;
    }
    else
    {
        cout<<"该商品不存在,请重新选择操作"<<endl;
    }
}
//商品销售
void Goods::sell()
{
    cout<<"请输入要销售的商品信息"<<endl;
    cout<<"编号:";
    cin>>numbering;
    for(int i=0;i<goodsList.size();i++)
    {   
        if(!goods.checkExist(numbering))
        {
            cout<<"输入错误,该商品不存在,请重新选择操作"<<endl;
        }
        else if(goodsList[i].numbering==numbering)
        {
            int stock;
            string date;
            cout<<"销售量:";
            cin>>stock;
            if(stock<0)
            {
                cout<<"操作错误,请重新操作"<<endl;
                break;
            }
            else
            {
                if(goodsList[i].stock<stock)
                {
                    cout<<"商品库存不足,剩余"<<goodsList[i].stock<<goodsList[i].unit<<endl;
                    cout<<"1.全部出售"<<endl;
                    cout<<"2.返回操作界面"<<endl;
                    int n;
                    cin>>n;
                    switch (n)
                    {
                    case 1:
                        {
                            goodsList[i].stock=0;
                            cout<<"请输入日期：";
                            cin>>date;
                            goodsList[i].date=date;
                            cout<<"商品已全部出售,请尽快进货"<<endl;
                            goods.changeInf();
                            goods.addOperate(numbering,goodsList[i].name,goodsList[i].date);
                            break;
                        }
                    case 2:
                        {
                            break;
                        }
                    }
                    break;
                }
                else if(goodsList[i].stock>stock)
                {
                    cout<<"请输入日期：";
                    cin>>date;
                    goodsList[i].date=date;
                    goodsList[i].stock=goodsList[i].stock-stock;
                    cout<<"商品销售成功,剩余"<<goodsList[i].stock<<goodsList[i].unit<<endl;
                    goods.addOperate(numbering,goodsList[i].name,goodsList[i].date);
                    goods.changeInf();
                    break;
                }
                else if(goodsList[i].stock==stock)
                {
                    cout<<"请输入日期：";
                    cin>>date;
                    goodsList[i].date=date;
                    goodsList[i].stock=0;
                    cout<<"商品已全部售出,请尽快进货"<<endl;
                    goods.addOperate(numbering,goodsList[i].name,goodsList[i].date);
                    goods.changeInf();
                    break;
                }
                
            }
        }

    }

}
//商品进货
void Goods::shipping()
{
    cout<<"请输入要进货的商品信息"<<endl;
    cout<<"编号:";
    cin>>numbering;
    for(int i=0;i<goodsList.size();i++)
    {
        if(!goods.checkExist(numbering))
        {
            cout<<"输入错误,该商品不存在,请重新选择操作"<<endl;
        }
        else if(goodsList[i].numbering==numbering)
        {
            int stock;
            string date;
            cout<<"进货量:";
            cin>>stock;
            if(stock<0)
            {
                cout<<"操作错误,请重新操作"<<endl;
                break;
            }
            else
            {
                cout<<"请输入日期：";
                cin>>date;
                cout<<endl;
                cout<<"进货成功"<<endl;
                goodsList[i].date=date;
                goodsList[i].stock=goodsList[i].stock+stock;
                cout<<"现有库存量:"<<goodsList[i].stock<<goodsList[i].unit<<endl;
                goods.addOperate(numbering,goodsList[i].name,goodsList[i].date);
                goods.changeInf();
                break;
            }
        }
    }
}
//显示商品信息
void Goods::proInformation()
{
    cout<<"------------------------------商品信息------------------------------"<<endl;
    //cout<<left<<setw(15)<<"编号"<<setw(15)<<"名称"<<setw(15)<<"价格"<<setw(15)<<"单位"<<setw(15)<<"库存量"<<endl;
    /*for(int i=0;i<goodsList.size();i++)
    {
        cout<<left<<setw(15)<<goodsList[i].numbering<<setw(15)<<goodsList[i].name<<setw(15)<<goodsList[i].price<<setw(15)<<goodsList[i].unit<<setw(15)<<goodsList[i].stock<<endl;
    }*/
    string filename="D:/c++/Me/GoodsInf.txt";
    ifstream input(filename);
    if(!input.is_open())
    {
        cout<<"无法打开文件"<<endl;

    }
    string line;
    while(getline(input,line))
    {
        cout<<line<<endl;
    }
    input.close();
    string date;
    cout<<"请输入日期：";
    cin>>date;
    goods.addOperate("**","****",date);
}
//显示操作记录
void Goods::actionLog()
{
    string filename="D:/c++/Me/OperateInf.txt";
    ifstream input(filename);
    if(!input.is_open())
    {
        cout<<"无法打开文件"<<endl;

    }
    string line;
    while(getline(input,line))
    {
        cout<<line<<endl;
    }
    input.close();
    string date;
    cout<<"请输入日期：";
    cin>>date;
    goods.addOperate("****","****",date);

    
}
//追加记录
void Goods::addOperate(string Numbering,string Name,string DAte)
{

    string filename="D:/c++/Me/OperateInf.txt";
    ofstream file(filename,ios::app);
    string Opre;
    if(N==1)
        Opre="新增商品";
    else if(N==2)
        Opre="删除商品";
    else if(N==3)
        Opre="商品销售";
    else if(N==4)
        Opre="商品进货";
    else if(N==5)
        Opre="浏览商品信息";
    else if(N==6)
        Opre="浏览操作记录商品";
    else if(N==7)
        Opre="商品盘点";
    file<<left<<setw(20)<<Numbering<<setw(20)<<Name<<setw(20)<<Opre<<setw(20)<<userName<<setw(20)<<DAte<<endl;
    file.close();


}
//出库
void Goods::outbound()
{
    cout<<"请输入需要出入库的商品信息"<<endl;
    cout<<"编号:";
    cin>>numbering;
    for(int i=0;i<goodsList.size();i++)
    {   
        if(!goods.checkExist(numbering))
        {
           cout<<"输入错误,该商品不存在,请重新选择操作"<<endl;
        }
        else
        {
            int stock;
            cout<<"出库量:";
            cin>>stock;
            if(stock<0)
            {
                cout<<"输入错误,请重新操作"<<endl;
                break;
            }
            else
            {
                goodsList[i].stock=goodsList[i].stock-stock;
                cout<<"出库日期:";
                string Date;
                cin>>Date;
                goodsList[i].date=Date;
                string name=goodsList[i].name+"(出库)";
                goods.addOperate(numbering,name,goodsList[i].date);
                goods.changeInf();
                cout<<"出库成功";
                break;
            }
        }
    }

}
//入库
void Goods::storage()
{
   cout<<"请输入需要出入库的商品信息"<<endl;
    cout<<"编号:";
    cin>>numbering;
    for(int i=0;i<goodsList.size();i++)
    {   
        if(!goods.checkExist(numbering))
        {
           cout<<"输入错误,该商品不存在,请重新选择操作"<<endl;
        }
        else
        {
            int stock;
            cout<<"入库量:";
            cin>>stock;
            if(stock<0)
            {
                cout<<"输入错误,请重新操作"<<endl;
                break;
            }
            else
            {
                goodsList[i].stock=goodsList[i].stock-stock;
                cout<<"入库日期:";
                string Date;
                cin>>Date;
                string name=goodsList[i].name+"(入库)";
                goodsList[i].date=Date;
                goods.addOperate(numbering,name,goodsList[i].date);
                goods.changeInf();
                cout<<"入库成功";
                break;
            }
        }
    }
    
}
//盘点
void Goods::inventory()
{
    goods.proInformation();
    cout<<"--------------------------------------------------------------------"<<endl;
    cout<<"是否需要平库?(是/否):";
    string sf;
    cin>>sf;
    if(sf=="是")
    {
        string cr;
        cout<<"\033[31m您需要出库还是入库?(出库/入库):\033[0m";
        cin>>cr;
        if(cr=="出库")
        {
            goods.outbound();
        }
        if(cr=="入库")
        {
            goods.storage();
        }
        
    }

}
//新增商品信息追加
void Goods::newInf()
{
    string goodsInf("D:/c++/Me/GoodsInf.txt");
    ofstream file(goodsInf, ios::app);
    if(!file)
    {
        cout<<"Failed"<<endl;
    }
    file<<left<<setw(15)<<numbering<<setw(15)<<name<<setw(15)<<price<<setw(15)<<unit<<setw(15)<<stock<<endl;
    file.close();
}
//商品信息更改(在出库入库销售进货中更改的只有商品的数量)//全部更改
void Goods::changeInf()
{
    ofstream file("D:/c++/Me/GoodsInf.txt");
    if(file.is_open())
    {
        file<<left<<setw(15)<<"编号"<<setw(15)<<"名称"<<setw(15)<<"价格"<<setw(15)<<"单位"<<setw(15)<<"库存量"<<endl;
    }
    for(int i=0;i<goodsList.size();i++)
    {
        file<<left<<setw(15)<<goodsList[i].numbering<<setw(15)<<goodsList[i].name<<setw(15)<<goodsList[i].price<<setw(15)<<goodsList[i].unit<<setw(15)<<goodsList[i].stock<<endl;
    }
    file.close();
}
//删除商品信息
void Goods::deleteInf()
{


}
//退出登录
void Goods::signOut()
{
    system("cls");
    beforeLogin();
}
//操作函数
void operate();
//登录
bool login();
//主菜单
void menu();
//删除指定行及以下
void deleteConsoleLines(int startLine);
//登录前的菜单
void beforeLogin()
{
    cout<<"**********进销存管理系统**********"<<endl;
    cout<<"1.登录"<<endl;
    cout<<"2.退出"<<endl;
    int n;
    cin>>n;
    if(n==1)
    {
        while (!login()){}
        system("cls");
        cout<<"登录成功,欢迎使用进销存管理系统!"<<endl;//再这里写一个函数将用户账号和密码写入txt文件并命名为用户信息
        string userInfPath ="D:/c++/Me/user.txt";
        ofstream userInf;
        userInf.open(userInfPath);
        if(userInf.is_open())
        {
            userInf<<"用户："<<userName<<endl;
            userInf<<"密码："<<passWord<<endl;
            userInf.close();
        }
        menu();
    }
    if(n==2)
    {

        exit(0);
    }
}
//登录
bool login()
{
    string username,password;
    cout<<"请输入用户名：";
    cin>>username;
    cout<<"请输入密码：";
    cin>>password;
    if(username==userName&&password==passWord)
    {
        return true;
    }
    else
    {
        deleteConsoleLines(2);
        cout<<"用户名或密码错误,请重新登录"<<endl;
        cout<<"1.退出"<<endl;
        cout<<"2.重新登录"<<endl;
        int n;
        cin>>n;
        if(n==1)
        {
            exit(0);
        }
        else if(n==2){}
        return false;
    }
}
//主菜单显示
void menu()
{
    ofstream file2("D:/c++/Me/OperateInf.txt");
    if(file2.is_open())
    {
        file2<<left<<setw(20)<<"编号"<<setw(20)<<"名称"<<setw(20)<<"操作"<<setw(20)<<"操作人"<<setw(20)<<"日期"<<endl;
    }
    file2.close();
    ofstream file1("D:/c++/Me/GoodsInf.txt");
    if(file1.is_open())
    {
        file1<<left<<setw(15)<<"编号"<<setw(15)<<"名称"<<setw(15)<<"价格"<<setw(15)<<"单位"<<setw(15)<<"库存量"<<endl;
    }
    file1.close();
    cout<<"1.新增商品"<<endl;
    cout<<"2.删除商品"<<endl;
    cout<<"3.商品销售"<<endl;
    cout<<"4.商品进货"<<endl;
    cout<<"5.浏览商品信息"<<endl;
    cout<<"6.显示操作记录"<<endl;
    cout<<"7.盘点"<<endl;
    cout<<"8.退出登录"<<endl;
    operate();
}
//删除指定行及以下所有行操作
void deleteConsoleLines(int startLine) 
{
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD consoleSize, charsWritten;
    COORD cursorPos;
    // 获取屏幕缓冲区信息
    GetConsoleScreenBufferInfo(stdoutHandle, &csbi);
    consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    // 定位光标到指定行的首个字符位置
    cursorPos.X = 0;
    cursorPos.Y = startLine - 1;
    SetConsoleCursorPosition(stdoutHandle, cursorPos);
    // 填充空格来覆盖需要删除的行
    FillConsoleOutputCharacter(stdoutHandle, ' ', consoleSize, cursorPos, &charsWritten);
    FillConsoleOutputAttribute(stdoutHandle, csbi.wAttributes, consoleSize, cursorPos, &charsWritten);
    // 将光标移回开始行
    cursorPos.X = 0;
    cursorPos.Y = startLine - 1;
    SetConsoleCursorPosition(stdoutHandle, cursorPos);
}
//主菜单操作
void operate()
{
    cout<<"*************************************"<<endl;
    cout<<"请选择操作:";
    int n;
    cin>>n;
    N=n;
    if(n==1)
    {
        goods.addGoods();
        Sleep(800);
        deleteConsoleLines(10);
        operate();
    }
    else if(n==2)
    {
        goods.DeleteGoods();
        Sleep(800);
        deleteConsoleLines(10);
        operate();
    }
    else if(n==3)
    {
        goods.sell();
        Sleep(800);
        deleteConsoleLines(10);
        operate();
    }
    else if(n==4)
    {
        goods.shipping();
        Sleep(800);
        deleteConsoleLines(10);
        operate();
    }
    else if(n==5)
    {
        goods.proInformation();
        system("pause");
        deleteConsoleLines(10);
        operate();
        
    }
    else if(n==6)
    {
        goods.actionLog();
        system("pause");
        deleteConsoleLines(10);
        operate();
        
    }
    else if(n==7)
    {
        goods.inventory();
    }
    else if(n==8)
    {
        goods.signOut();
    }
    else
    {
        cout<<"输入错误,请重新选择操作";
        Sleep(800);
        deleteConsoleLines(10);
        operate();
    }

}


int main()
{
    beforeLogin();
    system("pause");
    return 0;
}

#include "Features.h"

using namespace std;

// Function use often
bool compareString(QString a1, QString b1) {
	string a = a1.toLocal8Bit().constData();
	string b = b1.toLocal8Bit().constData();
	if (a.length() == b.length()) {
		for (int i = 0; i < a.length(); ++i) {
			if (a[i] != b[i])
				return 0;
		}
	}
	else return 0;
	return 1;
}

bool checkUser(QString user) { // check if username exist in database, if exist return 1
	bool check = 0;
	CONNECTDB
		con->setSchema(DBACC);
	stmt = con->createStatement();
	res = stmt->executeQuery("select * from user_admin.account");
	while (res->next()) {
		SQLString checkUsername = res->getString("user_name");
		if (compareString(user, checkUsername.c_str()))
			check = 1;
	}
	delete stmt;
	delete res;
	DISCONNECTDB
		return check;
}

bool checkValidString(QString pass) { // check if username or pass string is invalid or valid (1 is valid)
	string a = pass.toLocal8Bit().constData();
	if (strlen(a.c_str()) < 6) return 0;
	for (int i = 0; i < a.length(); ++i) {
		if (a[i] < 47 || (a[i] > 57 && a[i] < 65) || (a[i] > 90 && a[i] < 97) || a[i] > 122) {
			return 0;
			break;
		}
	}
	return 1;
}

// Function in register
bool checkValidPhone(QString phonenumber) {
	if (phonenumber.size() < 8 || phonenumber.size() > 15)
		return 0;
	for (int i = 0; i < phonenumber.size(); ++i)
		if (!phonenumber[i].isDigit())
			return 0;
	return 1;
}

bool phoneCorrect(QString user, QString phone) {
	bool check = 0;
	CONNECTDB
		con->setSchema(DBACC);
	pstmt = con->prepareStatement("select phone from user_admin.account where binary user_name = (?)");
	pstmt->setString(1, user.toLocal8Bit().constData());
	res = pstmt->executeQuery();
	while (res->next()){
		SQLString checkphone = res->getString("phone");
		if (compareString(phone, checkphone.c_str()))
			check = 1;
	}
	delete pstmt;
	delete res;
	DISCONNECTDB
		return check;
}

void registToDB(QString user, QString pass, QString phone) {
	CONNECTDB
		con->setSchema(DBACC);
	pstmt = con->prepareStatement("call user_admin.getAccount(?,?,?)");
	pstmt->setString(1, user.toLocal8Bit().constData());
	pstmt->setString(2, pass.toLocal8Bit().constData());
	pstmt->setString(3, phone.toLocal8Bit().constData());
	pstmt->execute();
	delete pstmt;
	DISCONNECTDB
}

bool resetPassword(QString user,QString phone, QString pass) {
	if (checkUser(user) && phoneCorrect(user, phone))
	{
		CONNECTDB
			con->setSchema(DBACC);
		pstmt = con->prepareStatement("call user_admin.updatePassword(?,?)");
		pstmt->setString(1, user.toLocal8Bit().constData());
		pstmt->setString(2, pass.toLocal8Bit().constData());
		pstmt->execute();
		delete pstmt;
		DISCONNECTDB
			return 1;
	}
	return 0;
}

bool checkLogin(QString user, QString pass) {
	bool check = 0;
	CONNECTDB
		con->setSchema(DBACC);
	stmt = con->createStatement();
	res = stmt->executeQuery("select * from account");
	while (res->next()) {
		SQLString checkUsername = res->getString("user_name");
		SQLString checkPass = res->getString("pass");
		if (compareString(user, checkUsername.c_str()) && compareString(QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha1).toHex(), checkPass.c_str()))
			check = 1;
	}
	delete stmt;
	delete res;
	DISCONNECTDB
		return check;
}
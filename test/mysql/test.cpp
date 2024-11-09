#include <mysql/mysql.h>
#include <stdio.h>
#include <iostream>
#include <string>
// mysql -u debian-sys-maint -puUakoE1X8Ei6TqLJ
const char *db_host = "host.docker.internal";
const char *db_user = "debian-sys-maint";
const char *db_pass = "uUakoE1X8Ei6TqLJ";
const char *db_name = "data";
const int db_port = 3306;

int main() {
    MYSQL *mysql = mysql_init(NULL);  //��ʼ�����ݿ����ӱ���
    if (mysql == NULL) {
        printf("Error:%s\n", mysql_error(mysql));
        exit(1);
    }
    mysql = mysql_real_connect(mysql, db_host, db_user, db_pass, db_name, db_port, NULL,
                               0);  //����mysql
    if (mysql)
        printf("Success!\n");  //���ӳɹ�
    else
        printf("Failed!\n");

    //����������֮�󣬻�ȡӢ��û�����⣬�������ȡ���ľ������롣
    //�������ӵ�Ĭ���ַ���Ϊutf8��ԭʼĬ����latin1(������)
    mysql_set_character_set(mysql, "utf8");
    std::string sql = "select * from score";
    int n = mysql_query(mysql, sql.c_str());

    MYSQL_RES *res = mysql_store_result(mysql);
    if (res == nullptr) exit(0);

    int rows = mysql_num_rows(res);
    std::cout << "table has " << rows << " rows" << std::endl;

    int cols = mysql_num_fields(res);
    std::cout << "table has " << cols << " cols" << std::endl;

    MYSQL_FIELD *col_name = mysql_fetch_fields(res);
    for (int i = 0; i < cols; i++) {
        std::cout << col_name[i].name << "  |  ";
    }
    std::cout << std::endl;

    MYSQL_ROW line;                 //����MYSQL_ROW�������������������
    for (int i = 0; i < rows; i++)  //����rowsͨ��mysql_num_rows()��ȡ��
    {
        line = mysql_fetch_row(res);
        for (int j = 0; j < cols; j++)  //����colsͨ��mysql_num_fields()��ȡ��
        {
            std::cout << line[j] << " ";  //��ӡ��i�еĵ�j�����ݡ�
        }
        std::cout << std::endl;
    }

    // free(res); // ����Ҫ�Լ��ͷ�ָ����
    return 0;
}
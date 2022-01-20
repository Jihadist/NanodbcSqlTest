#include <iostream>
#include <nanodbc/nanodbc.h>

int main() try {
  auto const connstr =
      NANODBC_TEXT("Driver={ODBC Driver 17 for SQL "
                   "Server};Server=ip,port;Database=db_name;UID=username;PWD="
                   "password;");
  nanodbc::connection conn(connstr);

  nanodbc::execute(
      conn,
      NANODBC_TEXT(
          "IF(OBJECT_ID('tempdb..#test') is not null) drop table #test;"));
  nanodbc::execute(conn,
                   NANODBC_TEXT("create table #test (s1 int,s2 int ,s3 int);"));

  nanodbc::execute(conn, NANODBC_TEXT("insert into #test select 1,2,3; "));

  auto result = nanodbc::execute(conn, NANODBC_TEXT("select * from #test;"));
  while (result.next()) {
    auto i = result.get<int>(0);
    std::cout << i << std::endl;
  }
  return EXIT_SUCCESS;
} catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
}

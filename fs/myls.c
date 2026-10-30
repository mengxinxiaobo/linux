#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>      // 用于 getpwuid
#include <grp.h>      // 用于 getgrgid
#include <time.h>     // 用于 ctime

// 1. 获取文件类型
static int ftype(const char *fname){
    struct stat stares;
    // 注意：这里改用 lstat，否则无法识别符号链接本身
    if(lstat(fname,&stares) < 0){
        perror("lstat()");
        exit(1);
    }
    if(S_ISREG(stares.st_mode))  return '-';
    else if(S_ISDIR(stares.st_mode)) return 'd';
    else if(S_ISSOCK(stares.st_mode)) return 's';
    else if(S_ISCHR(stares.st_mode)) return 'c';
    else if(S_ISBLK(stares.st_mode)) return 'b';
    else if(S_ISFIFO(stares.st_mode)) return 'p';
    else if(S_ISLNK(stares.st_mode)) return 'l';
    else return '?';
}

// 2. 获取文件的大小
static off_t flen(const char *fname){
    struct stat statres;
    if (stat(fname,&statres) < 0) {
        perror("stat()");
        exit(1);
    }
    return statres.st_size;
}

// 3. 获取文件权限（数字形式）
static mode_t fmode_t(const char *fname){
    struct stat statres;
    if (stat(fname,&statres) < 0) {
        perror("stat()");
        exit(1);
    }
    return statres.st_mode & 0777; // 提取基本的 rwx 权限
}

// 4. 获取文件硬连接数
static nlink_t fnlink_num(const char *fname){
    struct stat statres;
    if (stat(fname,&statres) < 0) {
        perror("stat()");
        exit(1);
    }
    return statres.st_nlink;
}

// 5. 获取文件的所有者 UID
static uid_t fowner(const char *fname){
    struct stat statres;
    if (stat(fname,&statres) < 0) {
        perror("stat()");
        exit(1);
    }
    return statres.st_uid;
}

// 6. 获取文件的所有组 GID（改名，避免冲突）
static gid_t fgroup(const char *fname){
    struct stat statres;
    if (stat(fname,&statres) < 0) {
        perror("stat()");
        exit(1);
    }
    return statres.st_gid;
}

// 辅助函数：将数字权限转换为 rwx 字符串
void print_rwx(mode_t mode) {
    char str[10] = "rwxrwxrwx";
    for (int i = 0; i < 9; i++) {
        // 如果对应的权限位是 0，就把字母替换成 '-'
        if (!(mode & (1 << (8 - i)))) {
            str[i] = '-';
        }
    }
    printf("%s", str);
}

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    const char *file = argv[1];

    // 获取所有属性
    int type = ftype(file);
    mode_t perms = fmode_t(file);
    nlink_t links = fnlink_num(file);
    uid_t uid = fowner(file);
    gid_t gid = fgroup(file);
    off_t size = flen(file);

    // 获取用户名和组名
    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);
    const char *user_name = pw ? pw->pw_name : "unknown";
    const char *group_name = gr ? gr->gr_name : "unknown";

    // 获取时间（这里用 stat 重新获取一下时间，你也可以再封装一个 ftime 函数）
    struct stat st;
    stat(file, &st);
    char *time_str = ctime(&st.st_mtime);
    // ctime 返回的字符串带换行符，我们把它去掉
    time_str[24] = '\0'; 

    // 打印输出，格式模仿 ls -l
    // 输出：类型 权限 硬链接数 所有者 组 大小 时间 文件名
    printf("%c", type);
    print_rwx(perms);
    printf(" %ld %s %s %ld %s %s\n", 
           (long)links, user_name, group_name, (long)size, time_str, file);

    return 0;
}
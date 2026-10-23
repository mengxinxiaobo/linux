#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

// 定位到指定行的起始位置
// 成功返回该行的起始偏移量
// 失败返回 -1（errno 为 0 表示行不存在）
off_t seek_to_line(int fd, int target)
{
    char c;
    off_t pos = 0;
    int line = 1;

    // 将文件偏移量移动到文件开头
    if (lseek(fd, 0, SEEK_SET) == -1)
        return -1;

    // 逐个字符读取，直到找到目标行
    while (line < target) {

        ssize_t n = read(fd, &c, 1);

        // 到达文件末尾，说明目标行不存在
        if (n == 0) {
            errno = 0;
            return -1;
        }

        // 读取失败
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }

        pos++;  // 文件偏移量增加一个字节

        // 遇到换行符，说明进入下一行
        if (c == '\n')
            line++;
    }

    // 将文件偏移量移动到目标行起始位置
    return lseek(fd, pos, SEEK_SET);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    // fd1 用于读取文件内容
    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        perror("open fd1");
        exit(1);
    }

    // fd2 用于向原文件写入内容
    int fd2 = open(argv[1], O_WRONLY);
    if (fd2 < 0) {
        perror("open fd2");
        close(fd1);
        exit(1);
    }

    int n = 10;  // 要删除的行号

    // 获取文件总长度
    off_t file_size = lseek(fd1, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek");
        goto error;
    }

    // 找到第 10 行的起始位置
    errno = 0;
    off_t start = seek_to_line(fd1, n);

    if (start == -1 || start >= file_size) {
        fprintf(stderr, "第 10 行不存在或定位失败\n");
        goto error;
    }

    // 找到第 11 行的起始位置
    errno = 0;
    off_t end = seek_to_line(fd1, n + 1);

    // 如果第 11 行不存在，说明第 10 行是最后一行
    if (end == -1) {
        if (errno != 0) {
            perror("seek_to_line");
            goto error;
        }
        end = file_size;
    }

    // 计算第 10 行占用的字节数
    off_t len = end - start;

    // fd1 从第 11 行开始读取
    if (lseek(fd1, end, SEEK_SET) == -1) {
        perror("lseek fd1");
        goto error;
    }

    // fd2 从第 10 行开始写入
    if (lseek(fd2, start, SEEK_SET) == -1) {
        perror("lseek fd2");
        goto error;
    }

    char buf[4096];  // 用于存储读取的数据
    ssize_t size;

    // 不断读取第 11 行及后面的内容
    while ((size = read(fd1, buf, sizeof(buf))) != 0) {

        if (size < 0) {
            if (errno == EINTR)
                continue;
            perror("read");
            goto error;
        }

        // 将读取的数据写到第 10 行的位置
        ssize_t written = 0;

        // write 可能无法一次写完所有数据
        while (written < size) {

            ssize_t ret = write(fd2,
                                buf + written,
                                size - written);

            if (ret < 0 && errno == EINTR)
                continue;

            if (ret <= 0) {
                perror("write");
                goto error;
            }

            written += ret;
        }
    }

    // 覆盖完成后，文件末尾仍然存在多余的 len 字节
    // 使用 ftruncate 缩短文件长度
    if (ftruncate(fd2, file_size - len) == -1) {
        perror("ftruncate");
        goto error;
    }

    close(fd1);
    close(fd2);

    return 0;

error:
    close(fd1);
    close(fd2);
    return 1;
}
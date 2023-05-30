
#ifndef UTILS_H_
#define UTILS_H_


#define SET_BIT(REG,BIT) (REG=REG|(1<<BIT))
#define CLR_BIT(REG,BIT) (REG=REG&~(1<<BIT))
#define READ_BIT(REG,BIT) ((REG>>BIT)&1)
#define TOGGLE_BIT(REG,BIT) (REG=REG^(1<<BIT))

#define  SET_PORT(REG,DATA) (REG=REG|(DATA))
#define  CLR_PORT(REG)      REG=(REG&(~1))
#define  TOGGLE_PORT(REG)   (REG=REG^(1))



#endif /* UTILS_H_ */
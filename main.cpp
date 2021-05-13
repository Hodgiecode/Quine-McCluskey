#include <stdio.h>
#include <stdlib.h>
#include <vector>


struct term { 
	std::vector<int> id;
	std::vector<char> nabor;
};

int power(int base,int exp){
	int result=1;
	while (exp!=0){
		result=result*base;
		exp=exp-1;
	}
	
	return result;
}

int count_ones(std::vector<char> &tmp){
	int count=0;
	for (size_t i=0;i<tmp.size();i++){
		if (tmp[i]=='1'){
			count=count+1;
		}
	}
	
	return count;
}

int check_diff(std::vector<char> &tmp1, std::vector<char> &tmp2){
	int diff=0;
	int index=0;
	int count=0;
	
	if (count_ones(tmp1)==count_ones(tmp2)){
		return -1;
	}
	
	for (size_t i=0;i<tmp1.size();i++){
		if (tmp1[i]!=tmp2[i]){
			index=i;
			count=count+1;
		}
	}
	
	if (count==1){
		return index;
	} else {
		return -1;
	}
}

void print(std::vector<term> &vect){
	for (size_t i=0;i<vect.size();i++){
		for (size_t k=0;k<vect[i].id.size();k++){
			printf("%d ",vect[i].id[k]);
		}
		
		printf("| ");
		
		for (size_t j=0;j<vect[i].nabor.size();j++){
			printf("%c ",vect[i].nabor[j]);
		}
		
		printf("\n");
	}
	
	printf("\n");
}


void sort_vector_id(std::vector<term> &vect){
	int temp=0;
	for (size_t i=0;i<vect.size();i++){
			for (size_t j1=0;j1<vect[i].id.size();j1++){
				for (size_t j2=j1+1;j2<vect[i].id.size();j2++){
					if (vect[i].id[j1]>vect[i].id[j2]){
						temp=vect[i].id[j1];
						vect[i].id[j1]=vect[i].id[j2];
						vect[i].id[j2]=temp;
					}
				}
			}
		}
}


int main(int argc, char* argv[]){
	FILE *fin=NULL,*fout=NULL;
	char *fin_name;
	char *fout_name;
	
	int n=0,k=0,m=0;
	char c;
	
	std::vector<char> vec;
	std::vector<term> vect;
	std::vector<term> vect2;
	std::vector<char> tmp;
	std::vector<int> temp;
	std::vector<std::vector<int>> ones;
	
	if (argc<2){
		fin_name=(char*)"in.txt";
		fout_name=(char*)"out.txt";
	}
	
	if (argc==2){
		fin_name=argv[1];
		fout_name=(char*)"out.txt";
	}
	
	if (argc>2){
		fin_name=argv[1];
		fout_name=argv[2];
	}
	
	
	/*-- Чтение данных --- */
	fin=fopen(fin_name,"r");
	if (!fin){
		printf("Cannot open infile");
		return -1;
	}
	
	fscanf(fin,"%d %d",&k,&n);
	m=power(k,n);
	vec.resize(m);
	
	fscanf(fin, "%c", &c);
	for (int i=0;i<m;i++){
		fscanf(fin, "%c", &c);
		vec[i]=c;
		
	}
	
	fclose(fin);
	
	
	/*-- Строим таблицу истинности --*/
	
	for (int i=0;i<power(k,n);i++){
		for (int j=n-1;j>=0;j--){
			if (i & power(2,j)){
				tmp.push_back('1');
			} else {
				tmp.push_back('0');
			}
		}
		
		if (!tmp.empty() && vec[i]=='1'){
			term data;
			data.id.push_back(i);
			data.nabor=tmp;
			vect.push_back(data);
		}
		
		tmp.clear();
	}
	
	vec.clear();
	
	
	while (true){
		/*-- Комбинируем минтермы -- */
		for (size_t i=0;i<vect.size();i++){
			for (size_t j=i+1;j<vect.size();j++){
				int i3=check_diff(vect[i].nabor,vect[j].nabor);
				if (i3>-1){
					ones.push_back(vect[i].id);
					ones.push_back(vect[j].id);
					
					temp=vect[i].id;
					for (size_t z=0;z<vect[j].id.size();z++){
						temp.push_back(vect[j].id[z]);
					}
					
					tmp=vect[i].nabor;
					tmp[i3]='*';
					
					term data;
					data.id=temp;
					data.nabor=tmp;
					
					vect2.push_back(data);
				}
			}
		}
		
		
		/* -- поиск потеряшек -- */
		
		for (size_t i=0;i<vect.size();i++){
			int flag2=0;
			for (size_t z1=0;z1<ones.size();z1++){
				if (vect[i].id==ones[z1]){
					flag2=1;
					break;
				}
			}
			
			if (ones[0].size()>1 && flag2==0){
				vect2.push_back(vect[i]);
			}
		}
		
		ones.clear();
		
		/*-- сортировка и удаление дупликатов -- */
		
		sort_vector_id(vect2);
		for (size_t i=0;i<vect2.size();i++){
			for (size_t j=i+1;j<vect2.size();j++){
				if (vect2[i].id==vect2[j].id && vect2[i].nabor==vect2[j].nabor){
					vect2.erase(vect2.begin()+j);
				}
			}
		}
		
		
		/* Условие на выход после очередного этапа вектор данных не изменился не поменялся*/
		
		int similar=1;
		for (size_t i=0;i<vect.size();i++){
			for (size_t j=0;j<vect2.size();j++){
				if (i==j && vect[i].nabor!=vect2[j].nabor && vect[i].id!=vect2[j].id){
					similar=0;
					break;
				}
			}
		}
		
		
		if (similar==1){
			break;
		}
		
		vect=vect2;
		vect2.clear();
	}
	
	vect2=vect;
	printf("After first stage\n");
	print(vect);
	temp.clear();
	

	for (size_t i=0;i<vect2.size();i++){
		for (size_t j=i+1;j<vect2.size();j++){

			for(size_t z=0; z < vect2[i].nabor.size(); z++){
				if (((vect2[i].nabor[z] == '*') && (vect2[j].nabor[z] != '*')) || ((vect2[i].nabor[z] == '1') && (vect2[j].nabor[z] == '0')) )
					{ 
						term tmp_var;
						tmp_var = vect2[i];
						vect2[i] = vect2[j];
						vect2[j] = tmp_var;
						break;
					}
					
					if (((vect2[i].nabor[z] == '0') && (vect2[j].nabor[z] != '0')) || ((vect2[i].nabor[z] == '1') && (vect2[j].nabor[z] == '*')) )
						break;
				}
		}
	}
	
	printf("After lexicographical sort\n");
	printf("%d %d\n",n,vect2.size());
	print(vect2);
	
	fout=fopen(fout_name,"w");
	fprintf(fout,"%d %d\n",n,vect2.size());
	for (size_t i=0;i<vect2.size();i++){
		for (size_t j=0;j<vect2[i].nabor.size();j++){
			fprintf(fout,"%c",vect2[i].nabor[j]);
		}
		
		fprintf(fout,"\n");
	}
	
	fclose(fout);
	
	tmp.clear();
	temp.clear();
	vect.clear();
	vect2.clear();
	return 0;
}
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

int iscycle(vector <int>& vector, int matrix[20][20]) {
	if (vector.front() == vector.back()) { vector.erase(vector.end() - 1); return 1; }
	else if (matrix[vector.front()][vector.back()]) return 1;
	else return 0;
}

int inv(int& chislo, vector <int>& vector) {		//возвращает 1, если число chislo входит в массив vector
	if (find(vector.begin(), vector.end(), chislo) != vector.end()) return 1;
	else return 0;
}

void cycle(int v, vector <vector<int>>& adj_list, vector <int>& way) {  // v - вершина, с которой мы начинаем
	way.push_back(v);
	int ed = 0;
	int next_v;  // будет принимать значение потенциальной вершины-продолжения
	for (int j = 0; j < adj_list[v].size(); j++) {
		next_v = adj_list[v][j];
		if (inv(next_v, way)) continue;          // если нашелся данный элемент в векторе way
		else if (adj_list[next_v].size() == 1) {
			ed = 1;
			continue;
		}
		else {
			ed = 0;
			cycle(next_v, adj_list, way);
			break;
		}
	}
	if (ed) {
		ed = 0;
		for (int i = 0; i < adj_list[v].size(); i++)
			if (inv(adj_list[v][i], way)) continue;
			else cycle(adj_list[v][i], adj_list, way);
	}
}

void find_main_chain(vector <vector<int>>& adj_list, vector <int>& way) { //первый аргумент - список смежности, в котором описаны вершины; второй - для записи пройденного пути
	for (int v = 0; v < adj_list.size(); v++) {
		if (adj_list[v].size() != 0) {		// если вершина имеет потомков, то начинаем с нее
			cycle(v, adj_list, way);
			if (inv(v, adj_list[way.back()])) way.push_back(v);	// если мы нашли начальную вершину в продолжениях вершины, которая последняя в way, то 
			break;																											// мы ее добавляем в путь и получаем цикл
		}
		else continue;
	}
}

int cycle2(vector <int>& kusok, vector <int>& queue, vector <vector<int>>& adj_list) {
	for (int i = 0; i < kusok.size(); i++)
		if (!inv(kusok[i], queue))
			if (inv(kusok[i], adj_list[queue.back()])) {
				queue.push_back(kusok[i]); cycle2(kusok, queue, adj_list); break;
			}
			else continue;
	if (kusok.size() == queue.size()) return 1;
	return 0;

}

int checkcycle(vector <int>& kusok, vector <int>& way, vector <vector<int>>& adj_list) {  // проверяет, является ли вектор - циклом, если так, то строит его так, как цикл (начиная с контактной вершины) и возвращает 1
	vector <int> queue;
	for (int i = 0; i < kusok.size(); i++) if (inv(kusok[i], way)) { queue.push_back(kusok[i]); break; }
	if (cycle2(kusok, queue, adj_list))
		if (inv(queue.front(), adj_list[queue.back()])) {
			kusok = queue; return 1;
		}
	return 0;
}

int norm(vector <int>& kontact, vector<vector<int>>& grani) {			// определяет, есть ли такакя грань, которой принадлежат все к. вершины и возвращает ее номер, иначе -1
	for (int i = 0; i < grani.size(); i++) {
		int c = 0;	// считает кол-во контактных точек в текущей грани
		for (int k = 0; k < kontact.size(); k++) {
			if (inv(kontact[k], grani[i])) c++;
			else break;
		}
		if (c == kontact.size()) return i;
	}
	return -1;
}

int inmid(int& chislo, vector <vector<int>> grani) {		// проверяет, было ли когда-то число в середине какой-либо грани
	for (int i = 0; i < grani.size(); i++) {
		if (find(grani[i].begin(), grani[i].end(), chislo) != grani[i].begin() && find(grani[i].begin(), grani[i].end(), chislo) != grani[i].end() - 1) return 1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	ifstream in(argv[1]);
	if (!in)
		return -1;
	int razmer = 0, rebra = 0;
	int planar = 1;			// буль для определения планарный граф или нет
	in >> razmer;
	int matrix[20][20];
	//entering matrix
	for (int i = 0; i < razmer; ++i)
	{
		for (int j = 0; j < razmer; ++j)
		{
			in >> matrix[i][j];
		}
	}
	in.close();

	//////// Algorithm

	//создаю список смежности//
	vector <vector<int>> adj_list; vector <vector<int>> matrixcopy;
	adj_list.resize(razmer);				// в списке смежности razmer векторов
	matrixcopy.resize(razmer);				// как и в копии матрицы
	for (int i = 0; i < razmer; i++)
	{
		int pod = 1;
		for (int j = 0; j < razmer; j++) {
			if (matrix[i][j]) { adj_list[i].push_back(j); rebra++; pod = 0; }				// заполнили список смежности и заодно считаю рёбра
			matrixcopy[i].push_back(matrix[i][j]);								// заполнили копию осн. матрицы (копи-матрица)
		}
		if (pod) planar = 0;
	}

	

	if (planar) {
		if (razmer - rebra / 2 > 1) { planar = 0; }
		else {

			/*for (int i = 0; i < razmer; i++) {
				cout << "[ ";
				for (int j = 0; j < adj_list[i].size(); j++)			// печать смеж. списка -----
					cout << adj_list[i][j] << " ";
				cout << "] ";
			}*/

			/*/cout << '\n';
			for (int i = 0; i < razmer; i++) {
				for (int j = 0; j < razmer; j++) {						//  печать копи-матрицы -----
					cout << matrixcopy[i][j] << ' ';
				}
				cout << "\n";
			}*/


			vector <int> way;    // создали вектор для записи пути
			find_main_chain(adj_list, way);


			//cout << "\n";
			//for (int i = 0; i < way.size(); i++) cout << way[i] << ' ';			// печать основных вершин -----
			vector <int> nevosh;   //невошедшие вершины
			for (int i = 0; i < razmer; i++) if (inv(i, way)) continue;
			else nevosh.push_back(i);
			//cout << "\n";
			//for (int i = 0; i < nevosh.size(); i++) cout << nevosh[i] << ' ';	// вывожу невошедшие элементы ------

			for (int i = 0; i < way.size() - 1; i++)
				matrixcopy[way[i]][way[i + 1]] = matrixcopy[way[i + 1]][way[i]] = 0;  // обнуляю way-вершины в копи-матрице

			vector <vector <int>> adj_no_way; adj_no_way.resize(razmer);
			for (int i = 0; i < razmer; i++) {
				for (int j = 0; j < razmer; j++) {
					if (matrixcopy[i][j]) adj_no_way[i].push_back(j);
				}
			}

			/*cout << "\n\n";
			for (int i = 0; i < razmer; i++) {
				for (int j = 0; j < razmer; j++) {						//  печать копи-матрицы (без way-вершин) ------
					cout << matrixcopy[i][j] << ' ';
				}
				cout << "\n";
			}*/

			/// начинаем создавать куски ///

			vector <vector<int>> kuski;
			vector <vector<int>> komponenty;
			if (nevosh.size() > 1) 							// если число невошедших граней больше 1, то нужно распределить их на смежные и несмежные (на компоненты)
				for (int i = 0; i < nevosh.size(); i++) {
					int cont = 0;							 // нужен, для определения пропускать ли цифру из списка невошедших в. или нет
					for (int l = 0; l < komponenty.size(); l++) if (inv(nevosh[i], komponenty[l])) { cont = 1; break; }
					if (cont) continue;									// т.е., если это число мы уже добавляли в какую-то компоненту, то его пропускаем
					komponenty.push_back(vector <int>());					// добавляем новый массив
					komponenty.back().push_back(nevosh[i]);
					int k = i;
					for (int j = ++k; j < nevosh.size(); j++)
						if (matrixcopy[nevosh[i]][nevosh[j]]) {						// если эти две вершины смежные (по копи-матрице)
							matrixcopy[nevosh[i]][nevosh[j]] = matrixcopy[nevosh[j]][nevosh[i]] = 0;
							komponenty[i].push_back(nevosh[j]);
						}
				}

			/*cout << "\n\n";
			for (int i = 0; i < razmer; i++) {
				for (int j = 0; j < razmer; j++) {						//  печать копи-матрицы  ------
					cout << matrixcopy[i][j] << ' ';
				}
				cout << "\n";
			}*/

			/*cout << '\n';
			for (int i = 0; i < komponenty.size(); i++) {
				cout << "[ ";
				for (int j = 0; j < komponenty[i].size(); j++) cout << komponenty[i][j] << ' ';	// вывожу компоненты невошедших вершин -----
				cout << "] ";
			}*/

			int predel = way.size();
			if (way.front() == way.back()) predel--; // если way - цикл

			for (int i = 0; i < komponenty.size(); i++) {
				kuski.push_back(vector <int>());
				for (int j = 0; j < komponenty[i].size(); j++) {
					/*if (!in(komponenty[i][j], kuski.back()))*/ kuski.back().push_back(komponenty[i][j]);
					for (int k = 0; k < predel; k++)
						if (matrixcopy[(komponenty[i][j])][way[k]]) {
							matrixcopy[(komponenty[i][j])][way[k]] = matrixcopy[way[k]][(komponenty[i][j])] = 0;
							if (!inv(way[k], kuski.back())) kuski.back().push_back(way[k]); //типа, если мы до этого не добавляли этот элемент в данный кусок, то сейчас добавим
						}
				}
			}

			for (int i = 0; i < predel; i++) {			//проходимся по всем вершинам из way
				kuski.push_back(vector<int>());
				kuski.back().push_back(way[i]);
				for (int j = 0; j < adj_list[way[i]].size(); j++)  // идем по списку смежности каждой основной вершины
					if (matrixcopy[(way[i])][(adj_list[way[i]][j])]) {		//если эта вершина смежна с еще какой-то вершиной из way, то мы делаем еще один кусок
						matrixcopy[(way[i])][(adj_list[way[i]][j])] = matrixcopy[(adj_list[way[i]][j])][(way[i])] = 0;
						kuski.back().push_back(adj_list[way[i]][j]);
					}
				if (kuski.back().size() == 1) kuski.erase(kuski.end() - 1);
			}


			/*cout << '\n';
			for (int i = 0; i < kuski.size(); i++) {
				cout << "[ ";
				for (int j = 0; j < kuski[i].size(); j++) cout << kuski[i][j] << ' ';	// вывожу куски -----
				cout << "] ";
			}

			cout << "\n\n";
			for (int i = 0; i < razmer; i++) {
				for (int j = 0; j < razmer; j++) {						//  печать копи-матрицы  ------
					cout << matrixcopy[i][j] << ' ';
				}
				cout << "\n";
			}*/
			// проверим куски на циклы //
			//for (int i = 0; i < kuski.size(); i++) {
			//	if (kuski[i].size() > 2) checkcycle(kuski[i], way, adj_list);
			//}

			///   закончили с кусками   ///

			vector <vector<int>> grani;
			if (iscycle(way, matrix)) {
				grani.push_back(way);
				grani.push_back(way);
			}
			else grani.push_back(way);

			/*cout << "\n";
			for (int i = 0; i < razmer; i++) {
				cout << "[ ";
				for (int j = 0; j < adj_no_way[i].size(); j++)			// печать список смежности без way-вершин -----
					cout << adj_no_way[i][j] << " ";
				cout << "] ";
			}*/


			vector <int> kusok;
			while (!kuski.empty()) {			// вот здесь мы уже начинаем подключать куски к основному подграфу
				kusok = kuski.front();
				while (!kusok.empty()) {		//будем добавлять куски в порядке очереди
					if (kusok.size() == 1) {		// если кусок состоит из одной вершины, то мы смело ее добавляем в первую грань (лучше в начало, чтобы ее не дергали)
						grani[0].insert(grani[0].begin(), kusok[0]);
						kusok.clear();
					}
					else if (kusok.size() == 2) {			// если размерность == 2, то надо проверять на контактные вершины
						vector <int> kontact;			// контактные вершины
						for (int i = 0; i < kusok.size(); i++)
							if (inv(kusok[i], way))	kontact.push_back(kusok[i]);	// находим и запоминаем контактные вершины

						if (kontact.size() == 0) { planar = 0; break; }
						else if (kontact.size() == 1) {
							kusok.erase(find(kusok.begin(), kusok.end(), kontact[0]));   // удляю из kusok к. вершину для удобства
							for (int i = 0; i < grani.size(); i++) {
								if (inv(kontact[0], grani[i])) {			// нашли грань, где лежит наша единственная к. вершина
									grani[i].insert(grani[i].begin(), kusok[0]);
									kusok.clear();
									break;
								}
								else if (i == grani.size() - 1) planar = 0; // если грани так и не нашлись*
							}
						}
						else {		// если к. вершины две, при этом в куске 2 элемента
							int i = norm(kontact, grani);
							if (i >= 0) {
								int j = distance(grani[i].begin(), find(grani[i].begin(), grani[i].end(), kusok[0]));
								grani.push_back(vector <int>());		//добавили новую грань
								grani.back().push_back(grani[i][j++]);	//и добавили в эту грань первую к. вершину
								if (!iscycle(way, matrix)) {	// если way - не цикл, то новая цепь не будет отрезать буферные вершины из основной грани
									while (grani[i][j] != kontact[1]) {
										grani.back().push_back(grani[i][j++]);
									}
								}
								else {
									while (grani[i][j] != kontact[1]) {
										grani.back().push_back(grani[i][j]);		//промежуточную вершину мы добавили в новую грань и затем удалили, при этом, важно заметить,
										grani[i].erase(find(grani[i].begin(), grani[i].end(), grani[i][j]));	//мы не увеличивали j, так как после удаления след. число само "подойдёт" на j-ое место
									}
								}
								grani.back().push_back(grani[i][j]);
								kusok.clear();
							}
							else { planar = 0; kusok.clear(); }
						}
					}
					else {					// когда в куске > 2 вершин
						vector <int> kontact;
						for (int i = 0; i < kusok.size(); i++)
							if (inv(kusok[i], way))	kontact.push_back(kusok[i]);	// находим и запоминаем контактные вершины

						if (kontact.size() == 0) { planar = 0; break; }
						else if (checkcycle(kusok, way, adj_list)) {	// если кусок является циклом
							if (kontact.size() == 1) {		// если это цикл с 1-ой к. точкой
								for (int i = 0; i < grani.size(); i++)
									if (inv(kontact[0], grani[i])) {			// нашли грань, в которую входит наша единственнай к. точка, и теперь туда запихнем цикл
										grani.push_back(vector<int>()); grani.back() = kusok;  // создал новую грань, в которой весь цикл
										for (int j = 1; j < kusok.size(); j++) { grani[i].insert(grani[i].begin(), kusok[j]); }	// добавил в основную грань все вершины цикла
										break;
									}
									else if (i == grani.size() - 1) planar = 0;   // если грани так и не нашлись*
								kusok.clear();
							}
							else {			// если в цикле больше 1-ой к. точки 
								int i = norm(kontact, grani);
								if (i >= 0) {
									if (iscycle(way, matrix)) {		/// если основной граф - цикл, то будем добавлять по две цепи согласно правилу, когда у нас 2 к. вершины
										while (kusok.size() != 1) {																					// в куске из 2-ух элементов
											int j = distance(grani[i].begin(), find(grani[i].begin(), grani[i].end(), kusok[0]));
											grani.push_back(vector <int>());		//добавили новую грань
											grani.back().push_back(grani[i][j++]);	//и добавили в эту грань первую к. вершину
											while (grani[i][j] != kontact[1]) {
												grani.back().push_back(grani[i][j]);		//промежуточную вершину мы добавили в новую грань и затем удалили, при этом, важно заметить,
												grani[i].erase(find(grani[i].begin(), grani[i].end(), grani[i][j]));	//мы не увеличивали j, так как после удаления след. число само "подойдёт" на j-ое место
											}
											grani.back().push_back(grani[i][j]);
											kusok.erase(kusok.begin() + 1);
											kontact.erase(kontact.begin() + 1);
										}
										kusok.clear();
									}
									else {	// если основной граф не цикл, то будем делать немного по другому, используя буфер
										vector <int> bufer;
										while (kusok.size() != 1) {
											int j = distance(grani[i].begin(), find(grani[i].begin(), grani[i].end(), kusok[0]));
											grani.push_back(vector <int>());		//добавили новую грань
											grani.back().push_back(grani[i][j++]);	//и добавили в эту грань первую к. вершину
											while (grani[i][j] != kontact[1])
												if (!inv(grani[i][j++], bufer)) {
													grani.back().push_back(grani[i][j]); // если этой вершины нет в буфере, то мы ее добывляем в новую грань
													bufer.push_back(grani[i][j]);																		// и в буфер
												}
											grani.back().push_back(grani[i][j]);
											kusok.erase(kusok.begin() + 1);
											kontact.erase(kontact.begin() + 1);
										}
										kusok.clear();
									}
								}
								else {
									planar = 0; kusok.clear();
								}
							}
						}
						else {				// если в куске > 2 вершин, так оне еще и не цикл
							int i = norm(kontact, grani);
							if (i >= 0) {
								if (iscycle(way, matrix)) {  // основной граф - цикл
									if (kusok.size() == kontact.size()) {
										while (kusok.size() != 1) {
											int j = distance(grani[i].begin(), find(grani[i].begin(), grani[i].end(), kusok[0]));
											grani.push_back(vector <int>());		//добавили новую грань
											grani.back().push_back(grani[i][j++]);	//и добавили в эту грань первую к. вершину
											while (grani[i][j] != kontact[1]) {
												grani.back().push_back(grani[i][j]);		//промежуточную вершину мы добавили в новую грань и затем удалили, при этом, важно заметить,
												grani[i].erase(find(grani[i].begin(), grani[i].end(), grani[i][j]));	//мы не увеличивали j, так как после удаления след. число само "подойдёт" на j-ое место
											}
											grani.back().push_back(grani[i][j]);
											kusok.erase(kusok.begin() + 1);
											kontact.erase(kontact.begin() + 1);
										}
										kusok.clear();
									}
								}
								else {	// основной граф не цикл
									if (kusok.size() == kontact.size()) {
										while (kusok.size() != 1) {
											int j = distance(grani[i].begin(), find(grani[i].begin(), grani[i].end(), kusok[0]));
											grani.push_back(vector <int>());		//добавили новую грань
											grani.back().push_back(grani[i][j++]);	//и добавили в эту грань первую к. вершину
											while (grani[i][j] != kontact[1]) {
												grani.back().push_back(grani[i][j]);
												if (inmid(grani[i][j], grani)) grani[i].erase(find(grani[i].begin(), grani[i].end(), grani[i][j])); // так как оно где-то было в середине, то мы его удаляем из основной грани
												else j++;
											}
											grani.back().push_back(grani[i][j]);
											kusok.erase(kusok.begin() + 1);
											kontact.erase(kontact.begin() + 1);
										}
										kusok.clear();
									}
								}
							}
							else { planar = 0; kusok.clear(); }
						}
					}

				}// 1-ый while
				if (!planar) break;
				kuski.erase(kuski.begin());  //удаляем первый кусок

			}// основной while
		}
	}


	//Saving new matrix
	FILE* fo = fopen(argv[1], "w");
	if (planar) {
		fprintf(fo, "%d\n", razmer);
		for (int i = 0; i < razmer; i++) 
		{	
			for (int j = 0; j < razmer; j++)
				fprintf(fo, "%d ", matrix[i][j]);
			fprintf(fo, "\n");
		}
	}
	fprintf(fo, "Text:\n");
	if (planar) fprintf(fo, "Граф плоский!");
	else fprintf(fo, "Граф не планарный!");
	fclose(fo);
}

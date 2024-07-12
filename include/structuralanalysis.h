#pragma once
#include <unordered_map>

// My includes
#include "interface/globals.h"
#include "interface/utils.h"
#include "interface/beam.h"
#include "interface/node.h"

// Eigen
#include "Eigen/Dense"
#include "Eigen/Sparse"


class StructuralAnalysis{
public:

size_t gln = 3; // Número de graus de liberdade por nó
const std::vector<std::shared_ptr<Node>>& nodes;
const std::vector<Beam>& beams;
Eigen::MatrixXd bcn; // Beam code number - Cada linha é uma barra, cada coluna é a tradução do grau de liberdade local para global
std::unordered_map<std::shared_ptr<Node>, int> nodeId; // Mapeamento de nós para índices
std::vector<Eigen::SparseMatrix<double>> k;
std::vector<Eigen::SparseMatrix<double>> T;
std::vector<Eigen::SparseMatrix<double>> K;
Eigen::SparseMatrix<double> S;
Eigen::VectorXd P;


StructuralAnalysis(const std::vector<std::shared_ptr<Node>>& nodes, const std::vector<Beam>& beams) : nodes(nodes), beams(beams),
                                                                                                        k(beams.size(), Eigen::SparseMatrix<double>(3*2, 3*2)), 
                                                                                                        T(beams.size(), Eigen::SparseMatrix<double>(3*2, 3*2)), 
                                                                                                        K(beams.size(), Eigen::SparseMatrix<double>(3*2, 3*2)),
                                                                                                        S(Eigen::SparseMatrix<double>(3 * nodes.size(), 3 * nodes.size())),
                                                                                                        P(Eigen::VectorXd(3 * nodes.size())){
    P.setZero();

    for (size_t n = 0; n < nodes.size(); ++n) {
        nodeId[nodes[n]] = n;
    }

    
    
    bcn.resize(beams.size(), gln*2);
    for (size_t n = 0; n < beams.size(); ++n) {
        // Codigo de número das barras
        bcn(n, 0) = (nodeId[beams[n].startNode] + 1)*gln - 3;
        bcn(n, 1) = (nodeId[beams[n].startNode] + 1)*gln - 2;
        bcn(n, 2) = (nodeId[beams[n].startNode] + 1)*gln - 1;
        bcn(n, 3) = (nodeId[beams[n].endNode]   + 1)*gln - 3;
        bcn(n, 4) = (nodeId[beams[n].endNode]   + 1)*gln - 2;
        bcn(n, 5) = (nodeId[beams[n].endNode]   + 1)*gln - 1;

        // Matrix de rigidez da barra
        float E = beams[n].material->E;
        float A = beams[n].section->area;
        float I = beams[n].section->inertia;
        float L = beams[n].length;
        double Lexp2 = L*L;
        double Lexp3 = Lexp2*L;
        double sc1 =        E * A / L;
        double sc2 = 12.0 * E * I / Lexp3;
        double sc3 =  6.0 * E * I / Lexp2;
        double sc4 =  4.0 * E * I / L;
        double sc5 =  2.0 * E * I / L;

        // Matriz de Rigidez de cada barra
        k[n].insert(0, 0) =  sc1;                                                     k[n].insert(0, 3) = -sc1;




                                  k[n].insert(1, 1) =  sc2; k[n].insert(1, 2) =  sc3;                           k[n].insert(1, 4) = -sc2; k[n].insert(1, 5) =  sc3;
                                  


        
                                  k[n].insert(2, 1) =  sc3; k[n].insert(2, 2) =  sc4;                           k[n].insert(2, 4) = -sc3; k[n].insert(2, 5) =  sc5;
                                  


        
        k[n].insert(3, 0) = -sc1;                                                     k[n].insert(3, 3) =  sc1;
        


        
                                  k[n].insert(4, 1) = -sc2; k[n].insert(4, 2) = -sc3;                           k[n].insert(4, 4) =  sc2; k[n].insert(4, 5) = -sc3;
                                  


        
                                  k[n].insert(5, 1) =  sc3; k[n].insert(5, 2) =  sc5;                           k[n].insert(5, 4) = -sc3; k[n].insert(5, 5) =  sc4;


        // Matriz de transformação de cada barra - TEM ELEMENTOS TROCADOS POR CONTA DE QUE NA VISUALIZAÇÃO O y é POSITIVO PARA CIMA
        T[n].insert(0, 0) =  beams[n].r(0, 0);
        T[n].insert(0, 1) = -beams[n].r(0, 1);
        T[n].insert(1, 0) = -beams[n].r(1, 0);
        T[n].insert(1, 1) =  beams[n].r(1, 1);
        T[n].insert(2, 2) =  1.0;
        T[n].insert(3, 3) =  beams[n].r(0, 0);
        T[n].insert(3, 4) = -beams[n].r(0, 1);
        T[n].insert(4, 3) = -beams[n].r(1, 0);
        T[n].insert(4, 4) =  beams[n].r(1, 1);
        T[n].insert(5, 5) =  1.0;

        // Constroi a matriz de rigidez em coordenadas GLOBAIS
        K[n] = T[n].transpose() * k[n] * T[n]; // K = T' * k * T

        for (size_t i = 0; i < gln*2; ++i) {
            for (size_t j = 0; j < gln*2; ++j) {
                double valueToAdd = K[n].coeff(i, j);
                if (valueToAdd != 0.0) {
                    S.coeffRef(bcn(n, i), bcn(n, j)) += valueToAdd;
                }
            }
        }
    }



    for (size_t n = 0; n < nodes.size(); ++n) {
        // PREENCHER COM OS VALORES DOS CARREGAMENTOS NODAIS
    }

    // Adiciona apoios - FALTA ADICIONAR A PARTE DO CARREGAMENETO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (size_t n = 0; n < nodes.size(); ++n) {
         switch (nodes[n]->support) {
		    case Support::None:
                break;
            
            case Support::Lx:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;
                break;

            case Support::Ly:
                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;
                break;

            case Support::Lxy:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;
                break;

            case Support::Th:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                zeroRow(S, n*gln + 2);
                zeroColumn(S, n*gln + 2);
                S.coeffRef(n*gln + 2, n*gln + 2) = 1;
                break;

            case Support::Tv:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                zeroRow(S, n*gln + 2);
                zeroColumn(S, n*gln + 2);
                S.coeffRef(n*gln + 2, n*gln + 2) = 1;
                break;

			default:
		        std::cout << "Apoio desconhecido" << std::endl;
		        break;
		}
    }
}

};




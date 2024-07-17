#pragma once
#include <unordered_map>

// My includes
#include "interface/globals.h"
#include "interface/utils.h"
#include "interface/beam.h"
#include "interface/node.h"

// Eigen
#include "eigenpch.h"



class StructuralAnalysis{
public:
enum class Info
	{
        Success,
        Fail,
	};
size_t gln = 3; // Número de graus de liberdade por nó
std::vector<std::shared_ptr<Node>>* nodes;
std::vector<Beam>* beams;
Eigen::MatrixXi bcn; // Beam code number - Cada linha é uma barra, cada coluna é a tradução do grau de liberdade local para global
std::unordered_map<std::shared_ptr<Node>, int> nodeId; // Mapeamento de nós para índices
std::vector<Eigen::SparseMatrix<double>> k; // Matriz de rigidez de cada barra em coordenadas locais
std::vector<Eigen::SparseMatrix<double>> T; // Matriz de transformação
std::vector<Eigen::SparseMatrix<double>> K; // Matriz de rigidez de cada barra em coordenadas globais
Eigen::SparseMatrix<double> S; // Matriz de rigidez da estrutura
Eigen::VectorXd jl; // Vetor de carregamento em nós da estrutura em coordenadas globais
Eigen::VectorXd P; // Vetor de carregamento total da estrutura em coordenadas globais
Eigen::VectorXd d; // Vetor de deslocamentos da estrutura em coordenadas globais
Info info;
Eigen::MatrixXd v; // Deslocamento de cada barra em coordenadas globais
Eigen::MatrixXd F; // Forças nas extemidades das barras em coordenadas globais
Eigen::VectorXd R; // Vetor de reações de apoio


StructuralAnalysis() = default;


    
void Run(std::vector<std::shared_ptr<Node>>* newNodes, std::vector<Beam>* newBeams) {
    nodes = newNodes;
    beams = newBeams;

    nodeId.clear();
    k.clear();
    T.clear();
    K.clear();

    bcn.resize(beams->size(), 3*2);
    k.resize(beams->size(), Eigen::SparseMatrix<double>(3*2, 3*2));
    T.resize(beams->size(), Eigen::SparseMatrix<double>(3*2, 3*2));
    K.resize(beams->size(), Eigen::SparseMatrix<double>(3*2, 3*2));
    S=(Eigen::SparseMatrix<double>(3 * nodes->size(), 3 * nodes->size()));
    jl=(Eigen::VectorXd(3 * nodes->size()));
    P=(Eigen::VectorXd(3 * nodes->size()));
    d=(Eigen::VectorXd(3 * nodes->size()));
    v=(Eigen::MatrixXd(3*2, beams->size()));
    F=(Eigen::MatrixXd(3*2, beams->size()));
    R=(Eigen::VectorXd(3 * nodes->size()));

    
    if (nodes->empty() || beams->empty()) {
        info = Info::Fail;
        return;
    }
    
    
    for (size_t n = 0; n < nodes->size(); ++n) {
        nodeId[(*nodes)[n]] = n;
    }
    
    
    //bcn.resize(beams.size(), gln*2);
    for (size_t n = 0; n < beams->size(); ++n) {
        // Codigo de número das barras
        bcn(n, 0) = (nodeId[(*beams)[n].startNode] + 1)*gln - 3;
        bcn(n, 1) = (nodeId[(*beams)[n].startNode] + 1)*gln - 2;
        bcn(n, 2) = (nodeId[(*beams)[n].startNode] + 1)*gln - 1;
        bcn(n, 3) = (nodeId[(*beams)[n].endNode]   + 1)*gln - 3;
        bcn(n, 4) = (nodeId[(*beams)[n].endNode]   + 1)*gln - 2;
        bcn(n, 5) = (nodeId[(*beams)[n].endNode]   + 1)*gln - 1;

        // Matrix de rigidez da barra
        float E = (*beams)[n].material->E;
        float A = (*beams)[n].section->area;
        float I = (*beams)[n].section->inertia;
        float L = (*beams)[n].length;
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
        T[n].insert(0, 0) =  (*beams)[n].r(0, 0);
        T[n].insert(0, 1) = -(*beams)[n].r(0, 1);
        T[n].insert(1, 0) = -(*beams)[n].r(1, 0);
        T[n].insert(1, 1) =  (*beams)[n].r(1, 1);
        T[n].insert(2, 2) =  1.0;
        T[n].insert(3, 3) =  (*beams)[n].r(0, 0);
        T[n].insert(3, 4) = -(*beams)[n].r(0, 1);
        T[n].insert(4, 3) = -(*beams)[n].r(1, 0);
        T[n].insert(4, 4) =  (*beams)[n].r(1, 1);
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

    jl.setZero();
    for (size_t n = 0; n < nodes->size(); ++n) {
        jl.coeffRef(n*gln) = (*nodes)[n]->load.force.x;   //Força em x aplicado em nó
        jl.coeffRef(n*gln+1) = (*nodes)[n]->load.force.y; // Força em y aplicado em nó
        //P.coeffRef(n*gln+2) = ; // Momento aplicado em nó
    }

    P = jl; // Falta somar os carramentos distribuidos

    for (size_t n = 0; n < nodes->size(); ++n) {
         switch ((*nodes)[n]->support) {
		    case Support::None:
                break;
            
            case Support::Lx:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                break;

            case Support::Ly:
                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
                break;

            case Support::Lxy:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
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

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
                P.coeffRef(n*gln+2) = 0; // Momento aplicado em nó
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

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
                P.coeffRef(n*gln+2) = 0; // Momento aplicado em nó
                break;

			default:
		        std::cout << "Apoio desconhecido" << std::endl;
		        break;
		}



    }

    
    /*// IMPOSSÍVEL USAR ISSO, O TEMPO DE COMPILAÇÃO FICA 3 MINUTOS MAIOR!
    // Cria um solucionador de autovalores para matrizes não-simétricas
    Eigen::MatrixXd denseS = Eigen::MatrixXd(S);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(denseS);

    // Autovalores
    std::cout << "Autovalores:\n" << eigensolver.eigenvalues() << std::endl;
    // Autovetores
    std::cout << "Autovetores:\n" << eigensolver.eigenvectors() << std::endl;*/
    

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.analyzePattern(S);
    solver.factorize(S);
    if (solver.info() != Eigen::Success) {
        if (solver.info() == Eigen::NumericalIssue) {
            std::cerr << "Problema numérico detectado." << std::endl;
        } else if (solver.info() == Eigen::NoConvergence) {
            std::cerr << "O solver não convergiu." << std::endl;
        } else if (solver.info() == Eigen::InvalidInput) {
            std::cerr << "Entrada inválida para o solver." << std::endl;
        }
        return;
    }
    info = Info::Success;

    d = solver.solve(P);
    std::cout << "Deslocamentos (d):\n" << d << std::endl;
    

    R.setZero();
    for (size_t n = 0; n < beams->size(); ++n) {
        for (size_t i = 0; i < gln*2; ++i) {
            v(i, n) = d(bcn(n, i));
        }

        F.col(n) = K[n] * v.col(n);

        for (size_t i = 0; i < gln*2; ++i) {
            R(bcn(n,i)) += F(i, n);
        }
    }

    R = R - jl;



    std::cout << "Deslocamentos de cada barra (v):\n" << v << std::endl;
    std::cout << "Forças em cada barra (F):\n" << F << std::endl;
    std::cout << "Reações de apoio (R):\n" << R << std::endl;
}

void DrawReactions(Camera2D cam){
    for (size_t n = 0; n < nodes->size(); ++n) {
        float Rx = static_cast<float>(R(n * gln));
        float Rx_px = Rx * loadScale;
        drawArrow((*nodes)[n]->position, Vector2{Rx_px / cam.zoom, 0}, cam, false, raioNo * 2 / cam.zoom, raioNo / 1 / cam.zoom, raioNo * 0.5f / cam.zoom, DARKGREEN);

        float Ry = static_cast<float>(R(n * gln + 1));
        float Ry_px = Ry * loadScale;
        drawArrow((*nodes)[n]->position, Vector2{0, Ry_px / cam.zoom}, cam, false, raioNo * 2 / cam.zoom, raioNo / 1 / cam.zoom, raioNo * 0.5f / cam.zoom, DARKGREEN);



    }
}

};




pipeline {
  agent any
  stages {
    stage('Preparation') {
      steps {
        checkout scm
        sh 'git submodule update --init --recursive'
        sh 'mkdir build'
      }
    }
    stage('Run Build') {
      parallel {
        stage('Build on Linux - Debug') {
          agent {
            label 'linux'
          }
          steps {
            dir('build') {
              sh 'cmake -GNinja -DDebug=ON ..'
              sh 'cmake --build . -- -j2'
            }
          }
        }
        stage('Build on Linux - Official Build') {
          agent {
            label 'linux'
          }
          steps {
            dir('build') {
              sh 'cmake -GNinja -DOFFICIAL_BUILD=ON ..'
              sh 'cmake --build . -- -j2'
            }
          }
        }
      }
    }
    stage('Run Tests') {
      agent {
        label 'linux'
      }
      steps {
        dir('build') {
          sh 'ctest'
        }
      }
    }
  }
  
  post {
    always {
      cleanWs notFailBuild: true
    }
    
    failure {
      echo 'Build Failed'
    }
  }
}
pipeline {
  agent {
    docker { image 'ravenx8/ubuntu-dev' }
  }
  stages {
    stage('Preparation') {
      steps {
        checkout scm
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
              sh 'cd build && cmake -DDebug=ON ..'
              sh 'cd build && cmake --build . -- -j2'
            }
          }
        }
        stage('Build on Linux - Official Build') {
          agent {
            label 'linux'
          }
          steps {
            dir('build') {
              sh 'cd build && cmake -DOFFICIAL_BUILD=ON ..'
              sh 'cd build && cmake --build . -- -j2'
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
          sh 'cd build && ctest'
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
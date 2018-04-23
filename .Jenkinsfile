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
              sh 'cd build && cmake -DDebug ..'
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
        //stage('Build on Windows') { 
        //  agent {
        //    label 'windows'
        //  }
        //  steps {
        //    dir('build') {
        //      bat 'cd build && cmake --build .'
        //    }
        //  }
        //}
      }
    }
    stage('Run Tests') {
//      parallel {
        stage('Tests on Linux') {
          agent {
            label 'linux'
          }
          steps {
            dir('build') {
              sh 'cd build && ctest'
            }
          }
        }
//        stage('Tests on Windows') {
//          agent {
//            label 'windows'
//          }
//          steps {
//            dir('build') {
//              sh 'cd build && ctest'
//            }
//          }
//        }
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
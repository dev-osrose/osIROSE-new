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
    stage('Build on Linux') { 
      agent {
        label 'linux'
      }
      steps {
        dir('build') {
          sh 'cd build && cmake ..'
          sh 'cd build && cmake --build . -- -j2'
        }
      }
    }
//    stage('Build on Windows') { 
//      agent {
//        label 'windows'
//      }
//      steps {
//        dir('build') {
//          bat 'cd build && cmake --build .'
//        }
//      }
//    }
    stage('Test') { 
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
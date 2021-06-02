#!/usr/bin/env groovy
@Library(value="comms-shared@master", changelog=false)

buildLabel = 'ubuntu'

emailAddress = 'piseprashant@johndeere.com'

patches = '27.01.541 28.01.341 29.01.444 30.01.380'

currentVersion = ''


pipeline {
   agent none
   
   parameters {
      string(name: 'VERSION', defaultValue: '', description: 'Full version string for this build (Ex: 17.01.001)')
      string(name: 'PATCH_FROM', defaultValue: "${patches}", description: 'Versions from which to create patches')
      string(name: 'YUKON_TYPE', defaultValue: "${yukonType}", description: 'Board platform that is desired for patches to be created')
   }
   
   options {
      disableConcurrentBuilds()
      timestamps()
      buildDiscarder(logRotator(daysToKeepStr: '50'))
   }
   
   stages {
      stage('Create Patches') {
         when {
            expression {
               isStartedByUser()
            }
         }

         agent {
            label {
               label buildLabel
            }
         }

         steps {
            script {
               currentBuild.displayName = "${VERSION}"
               withEnv(["LIST=${PATCH_FROM}"]) {
                  runDockerCompose("jenkins-${YUKON_TYPE}", VERSION)
               }
            }
         }
         post {
            success {
               script {
                  def uploadSpec = """{
                     "files" : [
                        {
                           "pattern" : "Artifacts/*"
                        }
                     ]
                  }"""
                  if (isReleaseBranch('master')) {
                     artiUpload(uploadSpec)
                  } else {
                     archiveArtifacts "Artifacts/*"
                  }
                  
               }
            }
            failure {
               failureEmail(emailAddress)
            }
            fixed {
               fixedEmail(emailAddress)
            }
         }
      }
   }
}
